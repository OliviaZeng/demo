1、测试protobuf、goprotobuf、gogoprotobuf能否通用，即c++protobuf序列化，go和gogo能否反序列化成功；或者goprotobuf序列化，c++和gogo能否反序列化成功；或者gogoprotobuf序列化，c++和go能否反序列化成功；
2、测试方法设计： 利用google官网给出的数据类型表，在protobuf v2的基础上，测试optional和optional+default的序列化反序列
