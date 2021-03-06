// Code generated by protoc-gen-gogo. DO NOT EDIT.
// source: structdef-gogo.proto

/*
	Package goserbench is a generated protocol buffer package.

	It is generated from these files:
		structdef-gogo.proto

	It has these top-level messages:
		GogoProtoBufA
*/
package goserbench

import proto "github.com/gogo/protobuf/proto"
import fmt "fmt"
import math "math"
import _ "github.com/gogo/protobuf/gogoproto"

import io "io"
import github_com_gogo_protobuf_proto "github.com/gogo/protobuf/proto"

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

// This is a compile-time assertion to ensure that this generated file
// is compatible with the proto package it is being compiled against.
// A compilation error at this line likely means your copy of the
// proto package needs to be updated.
const _ = proto.GoGoProtoPackageIsVersion2 // please upgrade the proto package

type GogoProtoBufA struct {
	Name     int32 `protobuf:"varint,1,req,name=name" json:"name"`
	BirthDay int32 `protobuf:"varint,2,req,name=birthDay" json:"birthDay"`
	Phone    int32 `protobuf:"varint,3,req,name=phone" json:"phone"`
	Siblings int32 `protobuf:"varint,4,req,name=siblings" json:"siblings"`
	Spouse   int32 `protobuf:"varint,5,req,name=spouse" json:"spouse"`
	Money    int32 `protobuf:"varint,6,req,name=money" json:"money"`
}

func (m *GogoProtoBufA) Reset()                    { *m = GogoProtoBufA{} }
func (m *GogoProtoBufA) String() string            { return proto.CompactTextString(m) }
func (*GogoProtoBufA) ProtoMessage()               {}
func (*GogoProtoBufA) Descriptor() ([]byte, []int) { return fileDescriptorStructdefGogo, []int{0} }

func (m *GogoProtoBufA) GetName() int32 {
	if m != nil {
		return m.Name
	}
	return 0
}

func (m *GogoProtoBufA) GetBirthDay() int32 {
	if m != nil {
		return m.BirthDay
	}
	return 0
}

func (m *GogoProtoBufA) GetPhone() int32 {
	if m != nil {
		return m.Phone
	}
	return 0
}

func (m *GogoProtoBufA) GetSiblings() int32 {
	if m != nil {
		return m.Siblings
	}
	return 0
}

func (m *GogoProtoBufA) GetSpouse() int32 {
	if m != nil {
		return m.Spouse
	}
	return 0
}

func (m *GogoProtoBufA) GetMoney() int32 {
	if m != nil {
		return m.Money
	}
	return 0
}

func init() {
	proto.RegisterType((*GogoProtoBufA)(nil), "goserbench.GogoProtoBufA")
}
func (m *GogoProtoBufA) Marshal() (dAtA []byte, err error) {
	size := m.Size()
	dAtA = make([]byte, size)
	n, err := m.MarshalTo(dAtA)
	if err != nil {
		return nil, err
	}
	return dAtA[:n], nil
}

func (m *GogoProtoBufA) MarshalTo(dAtA []byte) (int, error) {
	var i int
	_ = i
	var l int
	_ = l
	dAtA[i] = 0x8
	i++
	i = encodeVarintStructdefGogo(dAtA, i, uint64(m.Name))
	dAtA[i] = 0x10
	i++
	i = encodeVarintStructdefGogo(dAtA, i, uint64(m.BirthDay))
	dAtA[i] = 0x18
	i++
	i = encodeVarintStructdefGogo(dAtA, i, uint64(m.Phone))
	dAtA[i] = 0x20
	i++
	i = encodeVarintStructdefGogo(dAtA, i, uint64(m.Siblings))
	dAtA[i] = 0x28
	i++
	i = encodeVarintStructdefGogo(dAtA, i, uint64(m.Spouse))
	dAtA[i] = 0x30
	i++
	i = encodeVarintStructdefGogo(dAtA, i, uint64(m.Money))
	return i, nil
}

func encodeVarintStructdefGogo(dAtA []byte, offset int, v uint64) int {
	for v >= 1<<7 {
		dAtA[offset] = uint8(v&0x7f | 0x80)
		v >>= 7
		offset++
	}
	dAtA[offset] = uint8(v)
	return offset + 1
}
func (m *GogoProtoBufA) Size() (n int) {
	var l int
	_ = l
	n += 1 + sovStructdefGogo(uint64(m.Name))
	n += 1 + sovStructdefGogo(uint64(m.BirthDay))
	n += 1 + sovStructdefGogo(uint64(m.Phone))
	n += 1 + sovStructdefGogo(uint64(m.Siblings))
	n += 1 + sovStructdefGogo(uint64(m.Spouse))
	n += 1 + sovStructdefGogo(uint64(m.Money))
	return n
}

func sovStructdefGogo(x uint64) (n int) {
	for {
		n++
		x >>= 7
		if x == 0 {
			break
		}
	}
	return n
}
func sozStructdefGogo(x uint64) (n int) {
	return sovStructdefGogo(uint64((x << 1) ^ uint64((int64(x) >> 63))))
}
func (m *GogoProtoBufA) Unmarshal(dAtA []byte) error {
	var hasFields [1]uint64
	l := len(dAtA)
	iNdEx := 0
	for iNdEx < l {
		preIndex := iNdEx
		var wire uint64
		for shift := uint(0); ; shift += 7 {
			if shift >= 64 {
				return ErrIntOverflowStructdefGogo
			}
			if iNdEx >= l {
				return io.ErrUnexpectedEOF
			}
			b := dAtA[iNdEx]
			iNdEx++
			wire |= (uint64(b) & 0x7F) << shift
			if b < 0x80 {
				break
			}
		}
		fieldNum := int32(wire >> 3)
		wireType := int(wire & 0x7)
		if wireType == 4 {
			return fmt.Errorf("proto: GogoProtoBufA: wiretype end group for non-group")
		}
		if fieldNum <= 0 {
			return fmt.Errorf("proto: GogoProtoBufA: illegal tag %d (wire type %d)", fieldNum, wire)
		}
		switch fieldNum {
		case 1:
			if wireType != 0 {
				return fmt.Errorf("proto: wrong wireType = %d for field Name", wireType)
			}
			m.Name = 0
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowStructdefGogo
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				m.Name |= (int32(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			hasFields[0] |= uint64(0x00000001)
		case 2:
			if wireType != 0 {
				return fmt.Errorf("proto: wrong wireType = %d for field BirthDay", wireType)
			}
			m.BirthDay = 0
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowStructdefGogo
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				m.BirthDay |= (int32(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			hasFields[0] |= uint64(0x00000002)
		case 3:
			if wireType != 0 {
				return fmt.Errorf("proto: wrong wireType = %d for field Phone", wireType)
			}
			m.Phone = 0
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowStructdefGogo
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				m.Phone |= (int32(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			hasFields[0] |= uint64(0x00000004)
		case 4:
			if wireType != 0 {
				return fmt.Errorf("proto: wrong wireType = %d for field Siblings", wireType)
			}
			m.Siblings = 0
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowStructdefGogo
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				m.Siblings |= (int32(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			hasFields[0] |= uint64(0x00000008)
		case 5:
			if wireType != 0 {
				return fmt.Errorf("proto: wrong wireType = %d for field Spouse", wireType)
			}
			m.Spouse = 0
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowStructdefGogo
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				m.Spouse |= (int32(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			hasFields[0] |= uint64(0x00000010)
		case 6:
			if wireType != 0 {
				return fmt.Errorf("proto: wrong wireType = %d for field Money", wireType)
			}
			m.Money = 0
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return ErrIntOverflowStructdefGogo
				}
				if iNdEx >= l {
					return io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				m.Money |= (int32(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			hasFields[0] |= uint64(0x00000020)
		default:
			iNdEx = preIndex
			skippy, err := skipStructdefGogo(dAtA[iNdEx:])
			if err != nil {
				return err
			}
			if skippy < 0 {
				return ErrInvalidLengthStructdefGogo
			}
			if (iNdEx + skippy) > l {
				return io.ErrUnexpectedEOF
			}
			iNdEx += skippy
		}
	}
	if hasFields[0]&uint64(0x00000001) == 0 {
		return github_com_gogo_protobuf_proto.NewRequiredNotSetError("name")
	}
	if hasFields[0]&uint64(0x00000002) == 0 {
		return github_com_gogo_protobuf_proto.NewRequiredNotSetError("birthDay")
	}
	if hasFields[0]&uint64(0x00000004) == 0 {
		return github_com_gogo_protobuf_proto.NewRequiredNotSetError("phone")
	}
	if hasFields[0]&uint64(0x00000008) == 0 {
		return github_com_gogo_protobuf_proto.NewRequiredNotSetError("siblings")
	}
	if hasFields[0]&uint64(0x00000010) == 0 {
		return github_com_gogo_protobuf_proto.NewRequiredNotSetError("spouse")
	}
	if hasFields[0]&uint64(0x00000020) == 0 {
		return github_com_gogo_protobuf_proto.NewRequiredNotSetError("money")
	}

	if iNdEx > l {
		return io.ErrUnexpectedEOF
	}
	return nil
}
func skipStructdefGogo(dAtA []byte) (n int, err error) {
	l := len(dAtA)
	iNdEx := 0
	for iNdEx < l {
		var wire uint64
		for shift := uint(0); ; shift += 7 {
			if shift >= 64 {
				return 0, ErrIntOverflowStructdefGogo
			}
			if iNdEx >= l {
				return 0, io.ErrUnexpectedEOF
			}
			b := dAtA[iNdEx]
			iNdEx++
			wire |= (uint64(b) & 0x7F) << shift
			if b < 0x80 {
				break
			}
		}
		wireType := int(wire & 0x7)
		switch wireType {
		case 0:
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return 0, ErrIntOverflowStructdefGogo
				}
				if iNdEx >= l {
					return 0, io.ErrUnexpectedEOF
				}
				iNdEx++
				if dAtA[iNdEx-1] < 0x80 {
					break
				}
			}
			return iNdEx, nil
		case 1:
			iNdEx += 8
			return iNdEx, nil
		case 2:
			var length int
			for shift := uint(0); ; shift += 7 {
				if shift >= 64 {
					return 0, ErrIntOverflowStructdefGogo
				}
				if iNdEx >= l {
					return 0, io.ErrUnexpectedEOF
				}
				b := dAtA[iNdEx]
				iNdEx++
				length |= (int(b) & 0x7F) << shift
				if b < 0x80 {
					break
				}
			}
			iNdEx += length
			if length < 0 {
				return 0, ErrInvalidLengthStructdefGogo
			}
			return iNdEx, nil
		case 3:
			for {
				var innerWire uint64
				var start int = iNdEx
				for shift := uint(0); ; shift += 7 {
					if shift >= 64 {
						return 0, ErrIntOverflowStructdefGogo
					}
					if iNdEx >= l {
						return 0, io.ErrUnexpectedEOF
					}
					b := dAtA[iNdEx]
					iNdEx++
					innerWire |= (uint64(b) & 0x7F) << shift
					if b < 0x80 {
						break
					}
				}
				innerWireType := int(innerWire & 0x7)
				if innerWireType == 4 {
					break
				}
				next, err := skipStructdefGogo(dAtA[start:])
				if err != nil {
					return 0, err
				}
				iNdEx = start + next
			}
			return iNdEx, nil
		case 4:
			return iNdEx, nil
		case 5:
			iNdEx += 4
			return iNdEx, nil
		default:
			return 0, fmt.Errorf("proto: illegal wireType %d", wireType)
		}
	}
	panic("unreachable")
}

var (
	ErrInvalidLengthStructdefGogo = fmt.Errorf("proto: negative length found during unmarshaling")
	ErrIntOverflowStructdefGogo   = fmt.Errorf("proto: integer overflow")
)

func init() { proto.RegisterFile("structdef-gogo.proto", fileDescriptorStructdefGogo) }

var fileDescriptorStructdefGogo = []byte{
	// 203 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xe2, 0x12, 0x29, 0x2e, 0x29, 0x2a,
	0x4d, 0x2e, 0x49, 0x49, 0x4d, 0xd3, 0x4d, 0xcf, 0x4f, 0xcf, 0xd7, 0x2b, 0x28, 0xca, 0x2f, 0xc9,
	0x17, 0xe2, 0x4a, 0xcf, 0x2f, 0x4e, 0x2d, 0x4a, 0x4a, 0xcd, 0x4b, 0xce, 0x90, 0xd2, 0x4d, 0xcf,
	0x2c, 0xc9, 0x28, 0x4d, 0xd2, 0x4b, 0xce, 0xcf, 0xd5, 0x07, 0x29, 0xd1, 0x07, 0x2b, 0x49, 0x2a,
	0x4d, 0x03, 0xf3, 0xc0, 0x1c, 0x7d, 0x84, 0x56, 0xa5, 0xc9, 0x8c, 0x5c, 0xbc, 0xee, 0xf9, 0xe9,
	0xf9, 0x01, 0x20, 0x9e, 0x53, 0x69, 0x9a, 0xa3, 0x90, 0x10, 0x17, 0x4b, 0x5e, 0x62, 0x6e, 0xaa,
	0x04, 0xa3, 0x02, 0x93, 0x06, 0xab, 0x13, 0xcb, 0x89, 0x7b, 0xf2, 0x0c, 0x42, 0x62, 0x5c, 0x1c,
	0x49, 0x99, 0x45, 0x25, 0x19, 0x2e, 0x89, 0x95, 0x12, 0x4c, 0x48, 0xe2, 0xc2, 0x5c, 0xac, 0x05,
	0x19, 0xf9, 0x79, 0xa9, 0x12, 0xcc, 0xa8, 0x8a, 0x8b, 0x33, 0x93, 0x72, 0x32, 0xf3, 0xd2, 0x8b,
	0x25, 0x58, 0x90, 0xc4, 0x45, 0xb8, 0xd8, 0x8a, 0x0b, 0xf2, 0x4b, 0x8b, 0x53, 0x25, 0x58, 0x51,
	0x8d, 0xc8, 0xcd, 0xcf, 0x4b, 0xad, 0x94, 0x60, 0x43, 0x08, 0x3a, 0x09, 0x9c, 0x78, 0x24, 0xc7,
	0x78, 0xe1, 0x91, 0x1c, 0xe3, 0x83, 0x47, 0x72, 0x8c, 0x13, 0x1e, 0xcb, 0x31, 0x00, 0x02, 0x00,
	0x00, 0xff, 0xff, 0x3c, 0x4c, 0xf5, 0xfe, 0xf9, 0x00, 0x00, 0x00,
}
