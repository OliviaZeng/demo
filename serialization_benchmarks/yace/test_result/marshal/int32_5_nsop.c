GencodeUnsafeMarshal           	10000000	       124 ns/op	      32 B/op	       1 allocs/op
GencodeMarshal                 	10000000	       136 ns/op	      32 B/op	       1 allocs/op
XDR2Marshal                    	10000000	       165 ns/op	      32 B/op	       1 allocs/op
ColferMarshal                  	10000000	       194 ns/op	      14 B/op	       1 allocs/op
GogoprotobufMarshal            	10000000	       201 ns/op	      16 B/op	       1 allocs/op
HproseMarshal                  	10000000	       207 ns/op	      13 B/op	       0 allocs/op
MsgpMarshal                    	 5000000	       255 ns/op	     112 B/op	       1 allocs/op
FlatBuffersMarshal             	 3000000	       457 ns/op	       0 B/op	       0 allocs/op
CapNProtoMarshal               	 3000000	       539 ns/op	      56 B/op	       2 allocs/op
EasyJsonMarshal                	 3000000	       592 ns/op	     128 B/op	       1 allocs/op
GoprotobufMarshal              	 2000000	       685 ns/op	     232 B/op	       3 allocs/op
GobMarshal                     	 1000000	      1244 ns/op	       0 B/op	       0 allocs/op
GoAvro2BinaryMarshal           	 1000000	      1495 ns/op	     368 B/op	       9 allocs/op
ProtobufMarshal                	 1000000	      1502 ns/op	     160 B/op	       7 allocs/op
XdrMarshal                     	 1000000	      1650 ns/op	     152 B/op	      15 allocs/op
UgorjiCodecMsgpackMarshal      	 1000000	      1962 ns/op	    1168 B/op	       4 allocs/op
UgorjiCodecBincMarshal         	 1000000	      2055 ns/op	    1200 B/op	       4 allocs/op
BsonMarshal                    	 1000000	      2067 ns/op	     288 B/op	       9 allocs/op
BinaryMarshal                  	 1000000	      2207 ns/op	     264 B/op	      20 allocs/op
JsonMarshal                    	  500000	      2577 ns/op	     496 B/op	       6 allocs/op
VmihailencoMsgpackMarshal      	  500000	      2823 ns/op	     192 B/op	       4 allocs/op
GoAvroMarshal                  	  300000	      4079 ns/op	     984 B/op	      31 allocs/op
SerealMarshal                  	  300000	      4244 ns/op	     560 B/op	      16 allocs/op
GoAvro2TextMarshal             	  300000	      4466 ns/op	    1040 B/op	      21 allocs/op
