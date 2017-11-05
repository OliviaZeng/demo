/*
package main

import (
	"log"
	//"github.com/golang/protobuf/proto"
	"github.com/gogo/protobuf/proto"
	"examplegogo"
	"fmt"
	"net"
	"unsafe"
	"bytes"
	"encoding/binary"
)

type head struct {
	lenght int32
	cmd int32
	types byte
}

func IntToBytes(n int32) []byte {
	x := int32(n)

	bytesBuffer := bytes.NewBuffer([]byte{})
	binary.Write(bytesBuffer, binary.BigEndian, x)
	return bytesBuffer.Bytes()
}

func CharToBytes(n byte) []byte {
	x := byte(n)

	bytesBuffer := bytes.NewBuffer([]byte{})
	binary.Write(bytesBuffer, binary.BigEndian, x)
	return bytesBuffer.Bytes()
}


func handleConn(conn net.Conn) {
	defer conn.Close()
	buf := make([]byte, 1024)
	for {
		n, err := conn.Read(buf)
		if err != nil {
			log.Fatal("Read err: ", err)
			return
		}

		fmt.Println("n=", n, "   data=", buf)

		pkgHead := head{}
		var k int32
		k = int32(unsafe.Sizeof(pkgHead.lenght)) + int32(unsafe.Sizeof(pkgHead.cmd)) + int32(unsafe.Sizeof(pkgHead.types))
		fmt.Printf("k=%d, %d, %d, %d\n", k, unsafe.Sizeof(pkgHead.lenght), unsafe.Sizeof(pkgHead.cmd), unsafe.Sizeof(pkgHead.types))
		body := buf[k:n]
		newTest := &examplegogo.OptionalTest{}
		err = proto.Unmarshal(body, newTest)
		if err != nil {
			log.Fatal("Unmarshal error: ", err)
		}
		fmt.Println("Optional Gogo unmar")
		fmt.Printf(" k1=%f\n", newTest.GetK1())
		fmt.Printf(" k2=%f\n", newTest.GetK2())
		fmt.Printf(" k3=%d\n", newTest.GetK3())
		fmt.Printf(" k4=%d\n", newTest.GetK4())
		fmt.Printf(" k5=%d\n", newTest.GetK5())
		fmt.Printf(" k6=%d\n", newTest.GetK6())
		fmt.Printf(" k7=%d\n", newTest.GetK7())
		fmt.Printf(" k8=%d\n", newTest.GetK8())
		fmt.Printf(" k9=%d\n", newTest.GetK9())
		fmt.Printf(" k10=%d\n", newTest.GetK10())
		fmt.Printf(" k11=%d\n", newTest.GetK11())
		fmt.Printf(" k12=%d\n", newTest.GetK12())
		fmt.Printf(" k13=%t\n", newTest.GetK13())
		fmt.Printf(" k14=%s\n", newTest.GetK14())
		fmt.Printf(" k15=%s\n", newTest.GetK15())
		fmt.Printf(" k16=%s->%d\n", examplegogo.FooType_name[int32(newTest.GetK16())], newTest.GetK16())

		fmt.Printf(" k17=%f\n", newTest.GetK17())
		fmt.Printf(" k18=%f\n", newTest.GetK18())
		fmt.Printf(" k19=%d\n", newTest.GetK19())
		fmt.Printf(" k20=%d\n", newTest.GetK20())
		fmt.Printf(" k21=%d\n", newTest.GetK21())
		fmt.Printf(" k22=%d\n", newTest.GetK22())
		fmt.Printf(" k23=%d\n", newTest.GetK23())
		fmt.Printf(" k24=%d\n", newTest.GetK24())
		fmt.Printf(" k25=%d\n", newTest.GetK25())
		fmt.Printf(" k26=%d\n", newTest.GetK26())
		fmt.Printf(" k27=%d\n", newTest.GetK27())
		fmt.Printf(" k28=%d\n", newTest.GetK28())
		fmt.Printf(" k29=%t\n", newTest.GetK29())
		fmt.Printf(" k30=%s\n", newTest.GetK30())
		fmt.Printf(" k31=%s\n", newTest.GetK31())
		fmt.Printf(" k32=%s->%d\n", examplegogo.FooType_name[int32(newTest.GetK32())], newTest.GetK32())

		//Option Gogo mar
		test := &examplegogo.OptionalTest{}
		test.K1=*(proto.Float64(21111.1111))
		test.K2=*(proto.Float32(32.22))
		test.K3=*(proto.Int32(4333))
		test.K4=*(proto.Int64(544444))
		test.K5=*(proto.Uint32(6555))
		test.K6=*(proto.Uint64(766666))
		test.K7=*(proto.Int32(-8777))
		test.K8=*(proto.Int64(-98888))
		test.K9=*(proto.Uint32(10999))
		test.K10=*(proto.Uint64(111010))
		test.K11=*(proto.Int32(1211))
		test.K12=*(proto.Int64(1312))
		test.K13=*(proto.Bool(true))
		test.K14=*(proto.String("nmlkjihgfedcba"))
		test.K15 = []byte("tttt")
        test.K16=examplegogo.FooType_Z.Enum()

		data, err := proto.Marshal(test)

		//head
		pkgHead.lenght = int32(unsafe.Sizeof(pkgHead.lenght)) + int32(unsafe.Sizeof(pkgHead.cmd)) + int32(unsafe.Sizeof(pkgHead.types)) + int32(len(data))
		pkgHead.cmd = 123
		pkgHead.types = 1

		fmt.Println("len", pkgHead.lenght)

		var i int
		var j int
		pk := make([]byte, 1024)

		slice := IntToBytes(pkgHead.lenght)
		j = len(slice)
		fmt.Println("slice= ", slice, i, j)
		copy(pk[0:], slice[0:j])
		i += j

		slice1 := IntToBytes(pkgHead.cmd)
		j = len(slice1)
		fmt.Println("slice1=", slice1, i, j)
		copy(pk[i:], slice1[:j])
		i += j

		slice2 := CharToBytes(byte(pkgHead.types))
		j = len(slice2)
		fmt.Println("slice2=", slice2, i, j)
		copy(pk[i:], slice2[:j])
		i += j


		fmt.Println("data=", data)
		copy(pk[i:], data[:len(data)])

		fmt.Println("pk=", pk)


		_, err2 := conn.Write(pk)
		if err2 != nil {
			log.Fatal("Write error: ", err2)
		}
	}
}


func main() {
	listener, err := net.Listen("tcp", "192.168.202.26:5555")
	if err != nil {
		log.Fatal("Listen err: ", err)
		return
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Fatal("Accept err: ", err)
			return
		}

		go handleConn(conn)
	}
}
*/

package main

import (
	"log"
	//"github.com/golang/protobuf/proto"
	"github.com/gogo/protobuf/proto"
	"examplegogo"
	"fmt"
	"net"
	"unsafe"
	"bytes"
	"encoding/binary"
)

type head struct {
	lenght int32
	cmd int32
	types byte
}

func IntToBytes(n int32) []byte {
	x := int32(n)

	bytesBuffer := bytes.NewBuffer([]byte{})
	binary.Write(bytesBuffer, binary.BigEndian, x)
	return bytesBuffer.Bytes()
}

func CharToBytes(n byte) []byte {
	x := byte(n)

	bytesBuffer := bytes.NewBuffer([]byte{})
	binary.Write(bytesBuffer, binary.BigEndian, x)

	return bytesBuffer.Bytes()
}

func handleConn(conn net.Conn) {
	defer conn.Close()

		//Option Gogo mar
		test := &examplegogo.OptionalTest{}
		test.K1=*(proto.Float64(21111.1111))
		test.K2=*(proto.Float32(32.22))
		test.K3=*(proto.Int32(4333))
		test.K4=*(proto.Int64(544444))
		test.K5=*(proto.Uint32(6555))
		test.K6=*(proto.Uint64(766666))
		test.K7=*(proto.Int32(-8777))
		test.K8=*(proto.Int64(-98888))
		test.K9=*(proto.Uint32(10999))
		test.K10=*(proto.Uint64(111010))
		test.K11=*(proto.Int32(1211))
		test.K12=*(proto.Int64(1312))
		test.K13=*(proto.Bool(true))
		test.K14=*(proto.String("nmlkjihgfedcba"))
		test.K15 = []byte("tttt")
        test.K16=examplegogo.FooType_Z.Enum()

		data, _ := proto.Marshal(test)

		//head
		pkgHead := head{}
		pkgHead.lenght = int32(unsafe.Sizeof(pkgHead.lenght)) + int32(unsafe.Sizeof(pkgHead.cmd)) + int32(unsafe.Sizeof(pkgHead.types)) + int32(len(data))
		pkgHead.cmd = 123
		pkgHead.types = 1

		fmt.Println("len", pkgHead.lenght)

		var i int
		var j int
		pk := make([]byte, 1024)

		slice := IntToBytes(pkgHead.lenght)
		j = len(slice)
		fmt.Println("slice= ", slice, i, j)
		copy(pk[0:], slice[0:j])
		i += j

		slice1 := IntToBytes(pkgHead.cmd)
		j = len(slice1)
		fmt.Println("slice1=", slice1, i, j)
		copy(pk[i:], slice1[:j])
		i += j

		slice2 := CharToBytes(byte(pkgHead.types))
		j = len(slice2)
		fmt.Println("slice2=", slice2, i, j)
		copy(pk[i:], slice2[:j])
		i += j

		fmt.Println("data=", data)
		copy(pk[i:], data[:len(data)])

		fmt.Println("pk=", pk)


		_, err2 := conn.Write(pk[:pkgHead.lenght])
		if err2 != nil {
			log.Fatal("Write error: ", err2)
		}
}

func main() {

	addr, err := net.ResolveTCPAddr("tcp4", "192.168.202.26:5555")
	if err != nil {
		log.Fatal("ResolveTCPAddr err: ", err)
		return
	}

	conn, err := net.DialTCP("tcp", nil, addr)
    if err != nil {
		log.Fatal("DialTCP err: ", err)
		return
	}
	defer conn.Close()

	go handleConn(conn)
	for {
	}

}
