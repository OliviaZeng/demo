GencodeUnsafeMarshal           	10000000	       123 ns/op	      32 B/op	       1 allocs/op
GencodeMarshal                 	10000000	       136 ns/op	      32 B/op	       1 allocs/op
XDR2Marshal                    	10000000	       160 ns/op	      32 B/op	       1 allocs/op
ColferMarshal                  	10000000	       184 ns/op	      32 B/op	       1 allocs/op
GogoprotobufMarshal            	10000000	       236 ns/op	      32 B/op	       1 allocs/op
MsgpMarshal                    	 5000000	       281 ns/op	     112 B/op	       1 allocs/op
FlatBuffersMarshal             	 3000000	       451 ns/op	       0 B/op	       0 allocs/op
CapNProtoMarshal               	 3000000	       537 ns/op	      56 B/op	       2 allocs/op
HproseMarshal                  	 2000000	       591 ns/op	     141 B/op	       0 allocs/op
EasyJsonMarshal                	 2000000	       638 ns/op	     128 B/op	       1 allocs/op
GoprotobufMarshal              	 2000000	       781 ns/op	     264 B/op	       4 allocs/op
GobMarshal                     	 1000000	      1317 ns/op	       0 B/op	       0 allocs/op
ProtobufMarshal                	 1000000	      1502 ns/op	     160 B/op	       7 allocs/op
GoAvro2BinaryMarshal           	 1000000	      1639 ns/op	     416 B/op	      11 allocs/op
XdrMarshal                     	 1000000	      1654 ns/op	     152 B/op	      15 allocs/op
BsonMarshal                    	 1000000	      2064 ns/op	     288 B/op	       9 allocs/op
UgorjiCodecMsgpackMarshal      	 1000000	      2138 ns/op	    1168 B/op	       4 allocs/op
UgorjiCodecBincMarshal         	 1000000	      2151 ns/op	    1200 B/op	       4 allocs/op
BinaryMarshal                  	 1000000	      2202 ns/op	     264 B/op	      20 allocs/op
JsonMarshal                    	  500000	      2897 ns/op	     512 B/op	       6 allocs/op
VmihailencoMsgpackMarshal      	  500000	      2973 ns/op	     192 B/op	       4 allocs/op
GoAvroMarshal                  	  300000	      4210 ns/op	     984 B/op	      31 allocs/op
SerealMarshal                  	  300000	      4430 ns/op	     704 B/op	      17 allocs/op
GoAvro2TextMarshal             	  300000	      4609 ns/op	    1040 B/op	      21 allocs/op
