package goserbench

import (
	"bytes"
	"encoding/gob"
	"encoding/json"
	"fmt"
	"math/rand"
	"os"
	"testing"


	"github.com/DeDiS/protobuf"
	"github.com/Sereal/Sereal/Go/sereal"
	"github.com/alecthomas/binary"
	"github.com/davecgh/go-xdr/xdr"
	"github.com/glycerine/go-capnproto"
	"github.com/gogo/protobuf/proto"
	flatbuffers "github.com/google/flatbuffers/go"
	"github.com/hprose/hprose-go"
	"github.com/tinylib/msgp/msgp"
	"github.com/ugorji/go/codec"
	"gopkg.in/mgo.v2/bson"
	vmihailenco "gopkg.in/vmihailenco/msgpack.v2"
)

var (
	validate = os.Getenv("VALIDATE")
)

func randString(l int) string {
	buf := make([]byte, l)
	for i := 0; i < (l+1)/2; i++ {
		buf[i] = byte(rand.Intn(256))
	}
	return fmt.Sprintf("%x", buf)[:l]
}

func generate() []*A {
	a := make([]*A, 0, 1000)
	for i := 0; i < 1000; i++ {
		a = append(a, &A{
			Name:     rand.Intn(5),
			BirthDay: rand.Intn(5),
			Phone:    rand.Intn(5),
			Siblings: rand.Intn(5),
			Spouse:   rand.Intn(5),
			Money:    rand.Intn(5),
		})
	}
	return a
}

type Serializer interface {
	Marshal(o interface{}) []byte
	Unmarshal(d []byte, o interface{}) error
	String() string
}

func benchMarshal(b *testing.B, s Serializer) {
	b.StopTimer()
	data := generate()
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		s.Marshal(data[rand.Intn(len(data))])
	}
}

func cmpTags(a, b map[string]string) bool {
	if len(a) != len(b) {
		return false
	}
	for k, v := range a {
		if bv, ok := b[k]; !ok || bv != v {
			return false
		}
	}
	return true
}

func cmpAliases(a, b []string) bool {
	if len(a) != len(b) {
		return false
	}
	for i, v := range a {
		if b[i] != v {
			return false
		}
	}
	return true
}

func benchUnmarshal(b *testing.B, s Serializer) {
	b.StopTimer()
	data := generate()
	ser := make([][]byte, len(data))
	for i, d := range data {
		o := s.Marshal(d)
		t := make([]byte, len(o))
		copy(t, o)
		ser[i] = t
	}
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		n := rand.Intn(len(ser))
		o := &A{}
		err := s.Unmarshal(ser[n], o)
		if err != nil {
			b.Fatalf("%s failed to unmarshal: %s (%s)", s, err, ser[n])
		}
		// Validate unmarshalled data.
		if validate != "" {
			i := data[n]
			correct := o.Name == i.Name && o.Phone == i.Phone && o.Siblings == i.Siblings && o.Spouse == i.Spouse && o.Money == i.Money && o.BirthDay == i.BirthDay //&& cmpTags(o.Tags, i.Tags) && cmpAliases(o.Aliases, i.Aliases)
			if !correct {
				b.Fatalf("unmarshaled object differed:\n%v\n%v", i, o)
			}
		}
	}
}

func TestMessage(t *testing.T) {
	println(`
A test suite for benchmarking various Go serialization methods.

See README.md for details on running the benchmarks.
`)

}

// github.com/tinylib/msgp

type MsgpSerializer struct{}

func (m MsgpSerializer) Marshal(o interface{}) []byte {
	out, _ := o.(msgp.Marshaler).MarshalMsg(nil)
	return out
}

func (m MsgpSerializer) Unmarshal(d []byte, o interface{}) error {
	_, err := o.(msgp.Unmarshaler).UnmarshalMsg(d)
	return err
}

func (m MsgpSerializer) String() string { return "Msgp" }

func BenchmarkMsgpMarshal(b *testing.B) {
	benchMarshal(b, MsgpSerializer{})
}

func BenchmarkMsgpUnmarshal(b *testing.B) {
	benchUnmarshal(b, MsgpSerializer{})
}

// gopkg.in/vmihailenco/msgpack.v2

type VmihailencoMsgpackSerializer struct{}

func (m VmihailencoMsgpackSerializer) Marshal(o interface{}) []byte {
	d, _ := vmihailenco.Marshal(o)
	return d
}

func (m VmihailencoMsgpackSerializer) Unmarshal(d []byte, o interface{}) error {
	return vmihailenco.Unmarshal(d, o)
}

func (m VmihailencoMsgpackSerializer) String() string {
	return "vmihailenco-msgpack"
}

func BenchmarkVmihailencoMsgpackMarshal(b *testing.B) {
	benchMarshal(b, VmihailencoMsgpackSerializer{})
}

func BenchmarkVmihailencoMsgpackUnmarshal(b *testing.B) {
	benchUnmarshal(b, VmihailencoMsgpackSerializer{})
}

// encoding/json

type JsonSerializer struct{}

func (j JsonSerializer) Marshal(o interface{}) []byte {
	d, _ := json.Marshal(o)
	return d
}

func (j JsonSerializer) Unmarshal(d []byte, o interface{}) error {
	return json.Unmarshal(d, o)
}

func (j JsonSerializer) String() string {
	return "json"
}

func BenchmarkJsonMarshal(b *testing.B) {
	benchMarshal(b, JsonSerializer{})
}

func BenchmarkJsonUnmarshal(b *testing.B) {
	benchUnmarshal(b, JsonSerializer{})
}

// github.com/mailru/easyjson

type EasyJSONSerializer struct{}

func (m EasyJSONSerializer) Marshal(o interface{}) []byte {
	out, _ := o.(*A).MarshalJSON()
	return out
}

func (m EasyJSONSerializer) Unmarshal(d []byte, o interface{}) error {
	err := o.(*A).UnmarshalJSON(d)
	return err
}

func (m EasyJSONSerializer) String() string { return "EasyJson" }

func BenchmarkEasyJsonMarshal(b *testing.B) {
	benchMarshal(b, EasyJSONSerializer{})
}

func BenchmarkEasyJsonUnmarshal(b *testing.B) {
	benchUnmarshal(b, EasyJSONSerializer{})
}

// gopkg.in/mgo.v2/bson

type BsonSerializer struct{}

func (m BsonSerializer) Marshal(o interface{}) []byte {
	d, _ := bson.Marshal(o)
	return d
}

func (m BsonSerializer) Unmarshal(d []byte, o interface{}) error {
	return bson.Unmarshal(d, o)
}

func (j BsonSerializer) String() string {
	return "bson"
}

func BenchmarkBsonMarshal(b *testing.B) {
	benchMarshal(b, BsonSerializer{})
}

func BenchmarkBsonUnmarshal(b *testing.B) {
	benchUnmarshal(b, BsonSerializer{})
}

// encoding/gob

type GobSerializer struct {
	b   bytes.Buffer
	enc *gob.Encoder
	dec *gob.Decoder
}

func (g *GobSerializer) Marshal(o interface{}) []byte {
	g.b.Reset()
	err := g.enc.Encode(o)
	if err != nil {
		panic(err)
	}
	return g.b.Bytes()
}

func (g *GobSerializer) Unmarshal(d []byte, o interface{}) error {
	g.b.Reset()
	g.b.Write(d)
	err := g.dec.Decode(o)
	return err
}

func (g GobSerializer) String() string {
	return "gob"
}

func NewGobSerializer() *GobSerializer {
	s := &GobSerializer{}
	s.enc = gob.NewEncoder(&s.b)
	s.dec = gob.NewDecoder(&s.b)
	err := s.enc.Encode(A{})
	if err != nil {
		panic(err)
	}
	var a A
	err = s.dec.Decode(&a)
	if err != nil {
		panic(err)
	}
	return s
}

func BenchmarkGobMarshal(b *testing.B) {
	s := NewGobSerializer()
	benchMarshal(b, s)
}

func BenchmarkGobUnmarshal(b *testing.B) {
	s := NewGobSerializer()
	benchUnmarshal(b, s)
}

// github.com/davecgh/go-xdr/xdr

type XdrSerializer struct{}

func (x XdrSerializer) Marshal(o interface{}) []byte {
	d, _ := xdr.Marshal(o)
	return d
}

func (x XdrSerializer) Unmarshal(d []byte, o interface{}) error {
	_, err := xdr.Unmarshal(d, o)
	return err
}

func (x XdrSerializer) String() string {
	return "xdr"
}

func BenchmarkXdrMarshal(b *testing.B) {
	benchMarshal(b, XdrSerializer{})
}

func BenchmarkXdrUnmarshal(b *testing.B) {
	benchUnmarshal(b, XdrSerializer{})
}

// github.com/ugorji/go/codec

type UgorjiCodecSerializer struct {
	name string
	h    codec.Handle
}

func NewUgorjiCodecSerializer(name string, h codec.Handle) *UgorjiCodecSerializer {
	return &UgorjiCodecSerializer{
		name: name,
		h:    h,
	}
}

func (u *UgorjiCodecSerializer) Marshal(o interface{}) []byte {
	var bs []byte
	codec.NewEncoderBytes(&bs, u.h).Encode(o)
	return bs
}

func (u *UgorjiCodecSerializer) Unmarshal(d []byte, o interface{}) error {
	return codec.NewDecoderBytes(d, u.h).Decode(o)
}

func (u *UgorjiCodecSerializer) String() string {
	return "ugorjicodec-" + u.name
}

func BenchmarkUgorjiCodecMsgpackMarshal(b *testing.B) {
	s := NewUgorjiCodecSerializer("msgpack", &codec.MsgpackHandle{})
	benchMarshal(b, s)
}

func BenchmarkUgorjiCodecMsgpackUnmarshal(b *testing.B) {
	s := NewUgorjiCodecSerializer("msgpack", &codec.MsgpackHandle{})
	benchUnmarshal(b, s)
}

func BenchmarkUgorjiCodecBincMarshal(b *testing.B) {
	h := &codec.BincHandle{}
	h.AsSymbols = codec.AsSymbolNone
	s := NewUgorjiCodecSerializer("binc", h)
	benchMarshal(b, s)
}

func BenchmarkUgorjiCodecBincUnmarshal(b *testing.B) {
	h := &codec.BincHandle{}
	h.AsSymbols = codec.AsSymbolNone
	s := NewUgorjiCodecSerializer("binc", h)
	benchUnmarshal(b, s)
}

// github.com/Sereal/Sereal/Go/sereal

type SerealSerializer struct{}

func (s SerealSerializer) Marshal(o interface{}) []byte {
	d, _ := sereal.Marshal(o)
	return d
}

func (s SerealSerializer) Unmarshal(d []byte, o interface{}) error {
	err := sereal.Unmarshal(d, o)
	return err
}

func (s SerealSerializer) String() string {
	return "sereal"
}

func BenchmarkSerealMarshal(b *testing.B) {
	benchMarshal(b, SerealSerializer{})
}

func BenchmarkSerealUnmarshal(b *testing.B) {
	benchUnmarshal(b, SerealSerializer{})
}

// github.com/alecthomas/binary

type BinarySerializer struct{}

func (b BinarySerializer) Marshal(o interface{}) []byte {
	d, _ := binary.Marshal(o)
	return d
}

func (b BinarySerializer) Unmarshal(d []byte, o interface{}) error {
	return binary.Unmarshal(d, o)
}

func (b BinarySerializer) String() string {
	return "binary"
}

func BenchmarkBinaryMarshal(b *testing.B) {
	benchMarshal(b, BinarySerializer{})
}

func BenchmarkBinaryUnmarshal(b *testing.B) {
	benchUnmarshal(b, BinarySerializer{})
}

// github.com/google/flatbuffers/go

type FlatBufferSerializer struct {
	builder *flatbuffers.Builder
}

func (s *FlatBufferSerializer) Marshal(o interface{}) []byte {
	a := o.(*A)
	builder := s.builder

	builder.Reset()
	FlatBufferAStart(builder)

	FlatBufferAAddName(builder, int32(a.Name))
	FlatBufferAAddPhone(builder, int32(a.Phone))
	FlatBufferAAddBirthDay(builder, int32(a.BirthDay))
	FlatBufferAAddSiblings(builder, int32(a.Siblings))
	FlatBufferAAddSpouse(builder, int32(a.Spouse))
	FlatBufferAAddMoney(builder, int32(a.Money))

	//name := builder.CreateString(a.Name)
	//phone := builder.CreateString(a.Phone)

	//FlatBufferAStart(builder)
	//FlatBufferAAddName(builder, name)
	//FlatBufferAAddPhone(builder, phone)
	//FlatBufferAAddBirthDay(builder, a.BirthDay.UnixNano())
	//FlatBufferAAddSiblings(builder, int32(a.Siblings))
	//var spouse byte
	//if a.Spouse {
	//	spouse = byte(1)
	//}
	//FlatBufferAAddSpouse(builder, spouse)
	//FlatBufferAAddMoney(builder, a.Money)

	builder.Finish(FlatBufferAEnd(builder))
	return builder.Bytes[builder.Head():]
}

func (s *FlatBufferSerializer) Unmarshal(d []byte, i interface{}) error {
	a := i.(*A)
	o := FlatBufferA{}
	o.Init(d, flatbuffers.GetUOffsetT(d))

	a.Name = int(o.Name())
	a.BirthDay = int(o.BirthDay())
	a.Phone = int(o.Phone())
	a.Siblings = int(o.Siblings())
	a.Spouse = int(o.Spouse())
	a.Money = int(o.Money())

	//a.Name = string(o.Name())
	//a.BirthDay = time.Unix(o.BirthDay(), 0)
	//a.Phone = string(o.Phone())
	//a.Siblings = int(o.Siblings())
	//a.Spouse = o.Spouse() == byte(1)
	//a.Money = o.Money()

	return nil
}

func (s *FlatBufferSerializer) String() string {
	return "FlatBuffer"
}

func BenchmarkFlatBuffersMarshal(b *testing.B) {
	benchMarshal(b, &FlatBufferSerializer{flatbuffers.NewBuilder(0)})
}

func BenchmarkFlatBuffersUnmarshal(b *testing.B) {
	benchUnmarshal(b, &FlatBufferSerializer{flatbuffers.NewBuilder(0)})
}

// github.com/glycerine/go-capnproto

type CapNProtoSerializer struct {
	buf []byte
	out *bytes.Buffer
}

func (x *CapNProtoSerializer) Marshal(o interface{}) []byte {
	a := o.(*A)
	s := capn.NewBuffer(x.buf)
	c := NewRootCapnpA(s)
	c.SetName(int32(a.Name))
	c.SetBirthDay(int32(a.BirthDay))
	c.SetPhone(int32(a.Phone))
	c.SetSiblings(int32(a.Siblings))
	c.SetSpouse(int32(a.Spouse))
	c.SetMoney(int32(a.Money))
	x.out.Reset()
	s.WriteTo(x.out)
	x.buf = []byte(s.Data)[:0]
	return x.out.Bytes()
}

func (x *CapNProtoSerializer) Unmarshal(d []byte, i interface{}) error {
	a := i.(*A)
	s, _, _ := capn.ReadFromMemoryZeroCopy(d)
	o := ReadRootCapnpA(s)
	a.Name = int(o.Name())
	a.BirthDay = int(o.BirthDay())
	a.Phone = int(o.Phone())
	a.Siblings = int(o.Siblings())
	a.Spouse = int(o.Spouse())
	a.Money = int(o.Money())
	return nil
}

func (x *CapNProtoSerializer) String() string {
	return "CapNProto"
}

func BenchmarkCapNProtoMarshal(b *testing.B) {
	benchMarshal(b, &CapNProtoSerializer{nil, &bytes.Buffer{}})
}

func BenchmarkCapNProtoUnmarshal(b *testing.B) {
	benchUnmarshal(b, &CapNProtoSerializer{nil, &bytes.Buffer{}})
}

// zombiezen.com/go/capnproto2

//type CapNProto2Serializer struct {
//	arena capnp.Arena
//}
//
//func (x *CapNProto2Serializer) Marshal(o interface{}) []byte {
//	a := o.(*A)
//	m, s, _ := capnp.NewMessage(x.arena)
//	c, _ := NewRootCapnp2A(s)
//	c.SetName(a.Name)
//	c.SetBirthDay(a.BirthDay.UnixNano())
//	c.SetPhone(a.Phone)
//	c.SetSiblings(int32(a.Siblings))
//	c.SetSpouse(a.Spouse)
//	c.SetMoney(a.Money)
//	b, _ := m.Marshal()
//	return b
//}
//
//func (x *CapNProto2Serializer) Unmarshal(d []byte, i interface{}) error {
//	a := i.(*A)
//	m, _ := capnp.Unmarshal(d)
//	o, _ := ReadRootCapnp2A(m)
//	a.Name, _ = o.Name()
//	a.BirthDay = time.Unix(o.BirthDay(), 0)
//	a.Phone, _ = o.Phone()
//	a.Siblings = int(o.Siblings())
//	a.Spouse = o.Spouse()
//	a.Money = o.Money()
//	return nil
//}
//
//func (x *CapNProto2Serializer) String() string {
//	return "CapNProto2"
//}
//
//func BenchmarkCapNProto2Marshal(b *testing.B) {
//	benchMarshal(b, &CapNProto2Serializer{capnp.SingleSegment(nil)})
//}
//
//func BenchmarkCapNProto2Unmarshal(b *testing.B) {
//	benchUnmarshal(b, &CapNProto2Serializer{capnp.SingleSegment(nil)})
//}

// github.com/hprose/hprose-go/io

type HproseSerializer struct {
	writer *hprose.Writer
	reader *hprose.Reader
}

func (s *HproseSerializer) Marshal(o interface{}) []byte {
	a := o.(*A)
	writer := s.writer
	buf := writer.Stream.(*bytes.Buffer)
	l := buf.Len()
	writer.WriteInt64(int64(a.Name))
	writer.WriteInt64(int64(a.BirthDay))
	writer.WriteInt64(int64(a.Phone))
	writer.WriteInt64(int64(a.Siblings))
	writer.WriteInt64(int64(a.Spouse))
	writer.WriteInt64(int64(a.Money))
	return buf.Bytes()[l:]
}

func (s *HproseSerializer) Unmarshal(d []byte, i interface{}) error {
	o := i.(*A)
	reader := s.reader
	reader.Stream = &hprose.BytesReader{d, 0}
	o.Name, _ = reader.ReadInt()
	o.BirthDay, _ = reader.ReadInt()
	o.Phone, _ = reader.ReadInt()
	o.Siblings, _ = reader.ReadInt()
	o.Spouse, _ = reader.ReadInt()
	o.Money, _ = reader.ReadInt()
	return nil
}

func (s *HproseSerializer) String() string {
	return "Hprose"
}

func BenchmarkHproseMarshal(b *testing.B) {
	buf := new(bytes.Buffer)
	writer := hprose.NewWriter(buf, true)
	benchMarshal(b, &HproseSerializer{writer: writer})
}

func BenchmarkHproseUnmarshal(b *testing.B) {
	buf := new(bytes.Buffer)
	reader := hprose.NewReader(buf, true)
	bufw := new(bytes.Buffer)
	writer := hprose.NewWriter(bufw, true)
	benchUnmarshal(b, &HproseSerializer{writer: writer, reader: reader})
}

// github.com/DeDiS/protobuf

type ProtobufSerializer struct{}

func (m ProtobufSerializer) Marshal(o interface{}) []byte {
	d, _ := protobuf.Encode(o)
	return d
}

func (m ProtobufSerializer) Unmarshal(d []byte, o interface{}) error {
	return protobuf.Decode(d, o)
}

func (m ProtobufSerializer) String() string {
	return "protobuf"
}

func BenchmarkProtobufMarshal(b *testing.B) {
	benchMarshal(b, ProtobufSerializer{})
}

func BenchmarkProtobufUnmarshal(b *testing.B) {
	benchUnmarshal(b, ProtobufSerializer{})
}

// github.com/golang/protobuf

func generateProto() []*ProtoBufA {
	a := make([]*ProtoBufA, 0, 1000)
	for i := 0; i < 1000; i++ {
		a = append(a, &ProtoBufA{
			Name:     proto.Int32(rand.Int31n(5)),
			BirthDay: proto.Int32(rand.Int31n(5)),
			Phone:    proto.Int32(rand.Int31n(5)),
			Siblings: proto.Int32(rand.Int31n(5)),
			Spouse:   proto.Int32(rand.Int31n(5)),
			Money:    proto.Int32(rand.Int31n(5)),
		})
	}
	return a
}

func BenchmarkGoprotobufMarshal(b *testing.B) {
	b.StopTimer()
	data := generateProto()
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		proto.Marshal(data[rand.Intn(len(data))])
	}
}

func BenchmarkGoprotobufUnmarshal(b *testing.B) {
	b.StopTimer()
	data := generateProto()
	ser := make([][]byte, len(data))
	for i, d := range data {
		ser[i], _ = proto.Marshal(d)
	}
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		n := rand.Intn(len(ser))
		o := &ProtoBufA{}
		err := proto.Unmarshal(ser[n], o)
		if err != nil {
			b.Fatalf("goprotobuf failed to unmarshal: %s (%s)", err, ser[n])
		}
		// Validate unmarshalled data.
		if validate != "" {
			i := data[n]
			correct := *o.Name == *i.Name && *o.Phone == *i.Phone && *o.Siblings == *i.Siblings && *o.Spouse == *i.Spouse && *o.Money == *i.Money && *o.BirthDay == *i.BirthDay //&& cmpTags(o.Tags, i.Tags) && cmpAliases(o.Aliases, i.Aliases)
			if !correct {
				b.Fatalf("unmarshaled object differed:\n%v\n%v", i, o)
			}
		}
	}
}

// github.com/gogo/protobuf/proto

func generateGogoProto() []*GogoProtoBufA {
	a := make([]*GogoProtoBufA, 0, 1000)
	for i := 0; i < 1000; i++ {
		a = append(a, &GogoProtoBufA{
			Name:     rand.Int31n(5),
			BirthDay: rand.Int31n(5),
			Phone:    rand.Int31n(5),
			Siblings: rand.Int31n(5),
			Spouse:   rand.Int31n(5),
			Money:    rand.Int31n(5),
		})
	}
	return a
}

func BenchmarkGogoprotobufMarshal(b *testing.B) {
	b.StopTimer()
	data := generateGogoProto()
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		proto.Marshal(data[rand.Intn(len(data))])
	}
}

func BenchmarkGogoprotobufUnmarshal(b *testing.B) {
	b.StopTimer()
	data := generateGogoProto()
	ser := make([][]byte, len(data))
	for i, d := range data {
		ser[i], _ = proto.Marshal(d)
	}
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		n := rand.Intn(len(ser))
		o := &GogoProtoBufA{}
		err := proto.Unmarshal(ser[n], o)
		if err != nil {
			b.Fatalf("goprotobuf failed to unmarshal: %s (%s)", err, ser[n])
		}
		// Validate unmarshalled data.
		if validate != "" {
			i := data[n]
			correct := o.Name == i.Name && o.Phone == i.Phone && o.Siblings == i.Siblings && o.Spouse == i.Spouse && o.Money == i.Money && o.BirthDay == i.BirthDay //&& cmpTags(o.Tags, i.Tags) && cmpAliases(o.Aliases, i.Aliases)
			if !correct {
				b.Fatalf("unmarshaled object differed:\n%v\n%v", i, o)
			}
		}
	}
}

// github.com/pascaldekloe/colfer

func generateColfer() []*ColferA {
	a := make([]*ColferA, 0, 1000)
	for i := 0; i < 1000; i++ {
		a = append(a, &ColferA{
			Name:     rand.Int31n(5),
			BirthDay: rand.Int31n(5),
			Phone:    rand.Int31n(5),
			Siblings: rand.Int31n(5),
			Spouse:   rand.Int31n(5),
			Money:    rand.Int31n(5),
		})
	}
	return a
}

func BenchmarkColferMarshal(b *testing.B) {
	data := generateColfer()
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		n := rand.Intn(len(data))
		_, err := data[n].MarshalBinary()
		if err != nil {
			b.Fatalf("Colfer failed to marshal %#v: %s (%s)", data[n], err)
		}
	}
}

func BenchmarkColferUnmarshal(b *testing.B) {
	data := generateColfer()
	ser := make([][]byte, len(data))
	for i, d := range data {
		var err error
		ser[i], err = d.MarshalBinary()
		if err != nil {
			b.Fatal(err)
		}
	}
	b.ReportAllocs()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		n := rand.Intn(len(ser))
		o := &ColferA{}
		if err := o.UnmarshalBinary(ser[n]); err != nil {
			b.Fatalf("Colfer failed to unmarshal %#v: %s (%s)", data[n], err)
		}
		if validate != "" {
			i := data[n]
			correct := o.Name == i.Name && o.Phone == i.Phone && o.Siblings == i.Siblings && o.Spouse == i.Spouse && o.Money == i.Money && o.BirthDay == i.BirthDay
			if !correct {
				b.Fatalf("unmarshaled object differed:\n%v\n%v", i, o)
			}
		}
	}
}

// github.com/andyleap/gencode

func generateGencode() []*GencodeA {
	a := make([]*GencodeA, 0, 1000)
	for i := 0; i < 1000; i++ {
		a = append(a, &GencodeA{
			Name:     rand.Int31n(5),
			BirthDay: rand.Int31n(5),
			Phone:    rand.Int31n(5),
			Siblings: rand.Int31n(5),
			Spouse:   rand.Int31n(5),
			Money:    rand.Int31n(5),
		})
	}
	return a
}

func BenchmarkGencodeMarshal(b *testing.B) {
	b.StopTimer()
	data := generateGencode()
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		data[rand.Intn(len(data))].Marshal(nil)
	}
}

func BenchmarkGencodeUnmarshal(b *testing.B) {
	b.StopTimer()
	data := generateGencode()
	ser := make([][]byte, len(data))
	for i, d := range data {
		ser[i], _ = d.Marshal(nil)
	}
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		n := rand.Intn(len(ser))
		o := &GencodeA{}
		_, err := o.Unmarshal(ser[n])
		if err != nil {
			b.Fatalf("gencode failed to unmarshal: %s (%s)", err, ser[n])
		}
		// Validate unmarshalled data.
		if validate != "" {
			i := data[n]
			correct := o.Name == i.Name && o.Phone == i.Phone && o.Siblings == i.Siblings && o.Spouse == i.Spouse && o.Money == i.Money && o.BirthDay == i.BirthDay //&& cmpTags(o.Tags, i.Tags) && cmpAliases(o.Aliases, i.Aliases)
			if !correct {
				b.Fatalf("unmarshaled object differed:\n%v\n%v", i, o)
			}
		}
	}
}

func generateGencodeUnsafe() []*GencodeUnsafeA {
	a := make([]*GencodeUnsafeA, 0, 1000)
	for i := 0; i < 1000; i++ {
		a = append(a, &GencodeUnsafeA{
			Name:     rand.Int31n(5),
			BirthDay: rand.Int31n(5),
			Phone:    rand.Int31n(5),
			Siblings: rand.Int31n(5),
			Spouse:   rand.Int31n(5),
			Money:    rand.Int31n(5),
		})
	}
	return a
}

func BenchmarkGencodeUnsafeMarshal(b *testing.B) {
	b.StopTimer()
	data := generateGencodeUnsafe()
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		data[rand.Intn(len(data))].Marshal(nil)
	}
}

func BenchmarkGencodeUnsafeUnmarshal(b *testing.B) {
	b.StopTimer()
	data := generateGencodeUnsafe()
	ser := make([][]byte, len(data))
	for i, d := range data {
		ser[i], _ = d.Marshal(nil)
	}
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		n := rand.Intn(len(ser))
		o := &GencodeUnsafeA{}
		_, err := o.Unmarshal(ser[n])
		if err != nil {
			b.Fatalf("gencode failed to unmarshal: %s (%s)", err, ser[n])
		}
		// Validate unmarshalled data.
		if validate != "" {
			i := data[n]
			correct := o.Name == i.Name && o.Phone == i.Phone && o.Siblings == i.Siblings && o.Spouse == i.Spouse && o.Money == i.Money && o.BirthDay == i.BirthDay //&& cmpTags(o.Tags, i.Tags) && cmpAliases(o.Aliases, i.Aliases)
			if !correct {
				b.Fatalf("unmarshaled object differed:\n%v\n%v", i, o)
			}
		}
	}
}

// github.com/calmh/xdr

func generateXDR() []*XDRA {
	a := make([]*XDRA, 0, 1000)
	for i := 0; i < 1000; i++ {
		a = append(a, &XDRA{
			Name:     rand.Int31n(5),
			BirthDay: rand.Int31n(5),
			Phone:    rand.Int31n(5),
			Siblings: rand.Int31n(5),
			Spouse:   rand.Int31n(5),
			Money:    rand.Int31n(5),
		})
	}
	return a
}

func BenchmarkXDR2Marshal(b *testing.B) {
	b.StopTimer()
	data := generateXDR()
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		data[rand.Intn(len(data))].MarshalXDR()
	}
}

func BenchmarkXDR2Unmarshal(b *testing.B) {
	b.StopTimer()
	data := generateXDR()
	ser := make([][]byte, len(data))
	for i, d := range data {
		ser[i] = d.MustMarshalXDR()
	}
	b.ReportAllocs()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		n := rand.Intn(len(ser))
		o := XDRA{}
		err := o.UnmarshalXDR(ser[n])
		if err != nil {
			b.Fatalf("xdr failed to unmarshal: %s (%s)", err, ser[n])
		}
		// Validate unmarshalled data.
		if validate != "" {
			i := data[n]
			correct := o.Name == i.Name && o.Phone == i.Phone && o.Siblings == i.Siblings && o.Spouse == i.Spouse && o.Money == i.Money && o.BirthDay == i.BirthDay
			if !correct {
				b.Fatalf("unmarshaled object differed:\n%v\n%v", i, o)
			}
		}
	}
}

// gopkg.in/linkedin/goavro.v1

func BenchmarkGoAvroMarshal(b *testing.B) {
	benchMarshal(b, NewAvroA())
}

func BenchmarkGoAvroUnmarshal(b *testing.B) {
	benchUnmarshal(b, NewAvroA())
}

// github.com/linkedin/goavro

func BenchmarkGoAvro2TextMarshal(b *testing.B) {
	benchMarshal(b, NewAvro2Txt())
}

func BenchmarkGoAvro2TextUnmarshal(b *testing.B) {
	benchUnmarshal(b, NewAvro2Txt())
}

func BenchmarkGoAvro2BinaryMarshal(b *testing.B) {
	benchMarshal(b, NewAvro2Bin())
}

func BenchmarkGoAvro2BinaryUnmarshal(b *testing.B) {
	benchUnmarshal(b, NewAvro2Bin())
}
