FlatBuffersMarshal             	 3000000	       452 ns/op	       0 B/op	       0 allocs/op
GobMarshal                     	 1000000	      1306 ns/op	       0 B/op	       0 allocs/op
ColferMarshal                  	10000000	       171 ns/op	      32 B/op	       1 allocs/op
GencodeMarshal                 	10000000	       137 ns/op	      32 B/op	       1 allocs/op
GencodeUnsafeMarshal           	10000000	       124 ns/op	      32 B/op	       1 allocs/op
GogoprotobufMarshal            	10000000	       226 ns/op	      32 B/op	       1 allocs/op
XDR2Marshal                    	10000000	       167 ns/op	      32 B/op	       1 allocs/op
CapNProtoMarshal               	 3000000	       535 ns/op	      56 B/op	       2 allocs/op
MsgpMarshal                    	 5000000	       271 ns/op	     112 B/op	       1 allocs/op
EasyJsonMarshal                	 2000000	       630 ns/op	     128 B/op	       1 allocs/op
HproseMarshal                  	 2000000	       604 ns/op	     141 B/op	       0 allocs/op
XdrMarshal                     	 1000000	      1665 ns/op	     152 B/op	      15 allocs/op
ProtobufMarshal                	 1000000	      1492 ns/op	     160 B/op	       7 allocs/op
VmihailencoMsgpackMarshal      	  500000	      2986 ns/op	     192 B/op	       4 allocs/op
BinaryMarshal                  	 1000000	      2209 ns/op	     264 B/op	      20 allocs/op
GoprotobufMarshal              	 2000000	       790 ns/op	     264 B/op	       4 allocs/op
BsonMarshal                    	 1000000	      2060 ns/op	     288 B/op	       9 allocs/op
GoAvro2BinaryMarshal           	 1000000	      1569 ns/op	     384 B/op	      10 allocs/op
JsonMarshal                    	  500000	      2917 ns/op	     512 B/op	       6 allocs/op
SerealMarshal                  	  300000	      4707 ns/op	     704 B/op	      17 allocs/op
GoAvroMarshal                  	  300000	      4194 ns/op	     984 B/op	      31 allocs/op
GoAvro2TextMarshal             	  300000	      4560 ns/op	    1040 B/op	      21 allocs/op
UgorjiCodecMsgpackMarshal      	 1000000	      2186 ns/op	    1168 B/op	       4 allocs/op
UgorjiCodecBincMarshal         	 1000000	      2223 ns/op	    1200 B/op	       4 allocs/op
