XDR2Unmarshal                  	20000000	       104 ns/op	       0 B/op	       0 allocs/op
GencodeUnsafeUnmarshal         	20000000	       114 ns/op	      32 B/op	       1 allocs/op
GencodeUnmarshal               	10000000	       124 ns/op	      32 B/op	       1 allocs/op
ColferUnmarshal                	10000000	       162 ns/op	      32 B/op	       1 allocs/op
GogoprotobufUnmarshal          	 5000000	       242 ns/op	      32 B/op	       1 allocs/op
FlatBuffersUnmarshal           	 5000000	       256 ns/op	      48 B/op	       1 allocs/op
MsgpUnmarshal                  	 3000000	       444 ns/op	      48 B/op	       1 allocs/op
CapNProtoUnmarshal             	 3000000	       473 ns/op	     136 B/op	       4 allocs/op
HproseUnmarshal                	 3000000	       594 ns/op	      80 B/op	       2 allocs/op
GoprotobufUnmarshal            	 2000000	       834 ns/op	     320 B/op	       3 allocs/op
ProtobufUnmarshal              	 2000000	       908 ns/op	      96 B/op	       7 allocs/op
EasyJsonUnmarshal              	 1000000	      1203 ns/op	      48 B/op	       1 allocs/op
GobUnmarshal                   	 1000000	      1353 ns/op	      48 B/op	       1 allocs/op
XdrUnmarshal                   	 1000000	      1398 ns/op	      96 B/op	       7 allocs/op
GoAvro2BinaryUnmarshal         	 1000000	      1513 ns/op	     408 B/op	       9 allocs/op
BinaryUnmarshal                	 1000000	      2264 ns/op	     256 B/op	      21 allocs/op
UgorjiCodecMsgpackUnmarshal    	  500000	      2370 ns/op	    1280 B/op	       3 allocs/op
UgorjiCodecBincUnmarshal       	  500000	      2557 ns/op	    1312 B/op	       3 allocs/op
BsonUnmarshal                  	  500000	      3067 ns/op	     160 B/op	      19 allocs/op
VmihailencoMsgpackUnmarshal    	  500000	      3351 ns/op	     288 B/op	      10 allocs/op
JsonUnmarshal                  	  500000	      3645 ns/op	     352 B/op	       4 allocs/op
SerealUnmarshal                	  300000	      4489 ns/op	     752 B/op	      24 allocs/op
GoAvro2TextUnmarshal           	  300000	      4498 ns/op	     652 B/op	      33 allocs/op
GoAvroUnmarshal                	  200000	     10233 ns/op	    3280 B/op	      83 allocs/op
