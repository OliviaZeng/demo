package goserbench

import (
	"bytes"
	"fmt"

	goavro2 "github.com/linkedin/goavro"
	goavro "gopkg.in/linkedin/goavro.v1"
)

type AvroA struct {
	record *goavro.Record
	codec  goavro.Codec
}

type Avro2Txt struct {
	codec *goavro2.Codec
}

type Avro2Bin struct {
	codec *goavro2.Codec
}

var (
	avroSchemaJSON = `
		{
		  "type": "record",
		  "name": "AvroA",
		  "doc:": "Schema for encoding/decoding sample message",
		  "namespace": "com.example",
		  "fields": [
		    {
		      "name": "name",
		      "type": "int"
		    },
		    {
		      "name": "birthday",
		      "type": "int"
		    },
		    {
		      "name": "phone",
		      "type": "int"
		    },
		    {
		      "name": "siblings",
		      "type": "int"
		    },
		    {
		      "name": "spouse",
		      "type": "int"
		    },
		    {
		      "name": "money",
		      "type": "int"
		    }
		  ]
		}
	`
)

func NewAvroA() *AvroA {
	rec, err := goavro.NewRecord(goavro.RecordSchema(avroSchemaJSON))
	if err != nil {
		fmt.Printf("Avro record initialization error: %v\n", err)
		return nil
	}
	codec, err := goavro.NewCodec(avroSchemaJSON)
	if err != nil {
		fmt.Printf("Avro codec initialization error: %v\n", err)
		return nil
	}
	return &AvroA{record: rec, codec: codec}
}

func (a *AvroA) Marshal(o interface{}) []byte {
	object := o.(*A)
	a.record.Set("name", int32(object.Name))
	a.record.Set("birthday", int32(object.BirthDay))
	a.record.Set("phone", int32(object.Phone))
	a.record.Set("siblings", int32(object.Siblings))
	a.record.Set("spouse", int32(object.Spouse))
	a.record.Set("money", int32(object.Money))
	b := new(bytes.Buffer)
	if err := a.codec.Encode(b, a.record); err != nil {
		fmt.Printf("Avro encoding error: %v\n", err)
		return []byte("")
	}
	return b.Bytes()
}

func (a *AvroA) Unmarshal(d []byte, o interface{}) error {
	object := o.(*A)
	b := bytes.NewBuffer(d)
	i, err := a.codec.Decode(b)
	if err != nil {
		fmt.Printf("Avro decoding error: %v\n", err)
		return err
	}
	rec := i.(*goavro.Record)
	temp, _ := rec.Get("name")
	object.Name = int(temp.(int32))
	temp, _ = rec.Get("birthday")
	object.BirthDay = int(temp.(int32))
	temp, _ = rec.Get("phone")
	object.Phone = int(temp.(int32))
	temp, _ = rec.Get("siblings")
	object.Siblings = int(temp.(int32))
	temp, _ = rec.Get("spouse")
	object.Spouse = int(temp.(int32))
	temp, _ = rec.Get("money")
	object.Money = int(temp.(int32))
	return nil
}

func (a *AvroA) String() string {
	return "GoAvro"
}

func avroMarshal(o interface{}, marshalFunc func([]byte, interface{}) ([]byte, error)) []byte {
	object := o.(*A)
	b, err := marshalFunc(nil, map[string]interface{}{
		"name":     int32(object.Name),
		"birthday": int32(object.Siblings),
		"phone":    int32(object.BirthDay),
		"siblings": int32(object.Siblings),
		"spouse":   int32(object.Spouse),
		"money":    int32(object.Money),
	})
	if err != nil {
		fmt.Printf("Avro2 encoding error: %v\n", err)
		return []byte("")
	}
	return b
}

func avroUnmarshal(d []byte, o interface{}, unmarshalFunc func([]byte) (interface{}, []byte, error)) error {
	object := o.(*A)
	r, _, err := unmarshalFunc(d)
	if err != nil {
		fmt.Printf("Avro2 decoding error: %v\n", err)
		return err
	}
	m := r.(map[string]interface{})
	object.Name = int(m["name"].(int32)) 
	object.BirthDay = int(m["birthday"].(int32))
	object.Phone = int(m["phone"].(int32))
	object.Siblings = int(m["siblings"].(int32))
	object.Spouse = int(m["spouse"].(int32))
	object.Money = int(m["money"].(int32))
	return nil
}

func NewAvro2Txt() *Avro2Txt {
	codec, err := goavro2.NewCodec(avroSchemaJSON)
	if err != nil {
		fmt.Printf("Avro2Txt codec initialization error: %v\n", err)
		return nil
	}
	return &Avro2Txt{codec: codec}
}

func (a *Avro2Txt) Marshal(o interface{}) []byte {
	return avroMarshal(o, a.codec.TextualFromNative)
}

func (a *Avro2Txt) Unmarshal(d []byte, o interface{}) error {
	return avroUnmarshal(d, o, a.codec.NativeFromTextual)
}

func (a *Avro2Txt) String() string {
	return "GoAvro2Text"
}

func NewAvro2Bin() *Avro2Bin {
	codec, err := goavro2.NewCodec(avroSchemaJSON)
	if err != nil {
		fmt.Printf("Avro2Bin codec initialization error: %v\n", err)
		return nil
	}
	return &Avro2Bin{codec: codec}
}

func (a *Avro2Bin) Marshal(o interface{}) []byte {
	return avroMarshal(o, a.codec.BinaryFromNative)
}

func (a *Avro2Bin) Unmarshal(d []byte, o interface{}) error {
	return avroUnmarshal(d, o, a.codec.NativeFromBinary)
}

func (a *Avro2Bin) String() string {
	return "GoAvro2Binary"
}
