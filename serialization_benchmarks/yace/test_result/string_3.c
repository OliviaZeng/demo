MsgpMarshal                    	 5000000	       294 ns/op	      96 B/op	       1 allocs/op
MsgpUnmarshal                  	 2000000	       838 ns/op	     115 B/op	       7 allocs/op
VmihailencoMsgpackMarshal      	  500000	      3363 ns/op	     336 B/op	       5 allocs/op
VmihailencoMsgpackUnmarshal    	  500000	      3791 ns/op	     352 B/op	      16 allocs/op
JsonMarshal                    	  500000	      2880 ns/op	     512 B/op	       6 allocs/op
JsonUnmarshal                  	  500000	      3514 ns/op	     424 B/op	      10 allocs/op
EasyJsonMarshal                	 2000000	       653 ns/op	     128 B/op	       1 allocs/op
EasyJsonUnmarshal              	 1000000	      1210 ns/op	     115 B/op	       7 allocs/op
BsonMarshal                    	  500000	      2679 ns/op	     384 B/op	       9 allocs/op
BsonUnmarshal                  	  500000	      3759 ns/op	     272 B/op	      25 allocs/op
GobMarshal                     	 1000000	      1291 ns/op	       0 B/op	       0 allocs/op
GobUnmarshal                   	 1000000	      1728 ns/op	     115 B/op	       7 allocs/op
XdrMarshal                     	 1000000	      2197 ns/op	     224 B/op	      22 allocs/op
XdrUnmarshal                   	 1000000	      1802 ns/op	     192 B/op	      13 allocs/op
UgorjiCodecMsgpackMarshal      	 1000000	      2248 ns/op	    1360 B/op	       5 allocs/op
UgorjiCodecMsgpackUnmarshal    	  500000	      2881 ns/op	    1347 B/op	       9 allocs/op
UgorjiCodecBincMarshal         	 1000000	      2288 ns/op	    1392 B/op	       5 allocs/op
UgorjiCodecBincUnmarshal       	  500000	      2780 ns/op	    1379 B/op	       9 allocs/op
SerealMarshal                  	  300000	      4445 ns/op	     800 B/op	      17 allocs/op
SerealUnmarshal                	  300000	      4775 ns/op	     824 B/op	      30 allocs/op
BinaryMarshal                  	 1000000	      2258 ns/op	     216 B/op	      14 allocs/op
BinaryUnmarshal                	  500000	      2578 ns/op	     256 B/op	      27 allocs/op
FlatBuffersMarshal             	 2000000	       697 ns/op	       0 B/op	       0 allocs/op
FlatBuffersUnmarshal           	 2000000	       710 ns/op	     115 B/op	       7 allocs/op
CapNProtoMarshal               	 1000000	      1279 ns/op	      56 B/op	       2 allocs/op
CapNProtoUnmarshal             	 1000000	      1134 ns/op	     203 B/op	      10 allocs/op
HproseMarshal                  	 1000000	      1208 ns/op	     233 B/op	       6 allocs/op
HproseUnmarshal                	  500000	      2471 ns/op	     320 B/op	      20 allocs/op
ProtobufMarshal                	 1000000	      1859 ns/op	     208 B/op	       7 allocs/op
ProtobufUnmarshal              	 1000000	      1270 ns/op	     192 B/op	      13 allocs/op
GoprotobufMarshal              	 2000000	       879 ns/op	     264 B/op	       4 allocs/op
GoprotobufUnmarshal            	 1000000	      1435 ns/op	     403 B/op	      14 allocs/op
GogoprotobufMarshal            	 5000000	       265 ns/op	      32 B/op	       1 allocs/op
GogoprotobufUnmarshal          	 3000000	       563 ns/op	     115 B/op	       7 allocs/op
ColferMarshal                  	10000000	       224 ns/op	      32 B/op	       1 allocs/op
ColferUnmarshal                	 3000000	       481 ns/op	     115 B/op	       7 allocs/op
GencodeMarshal                 	10000000	       213 ns/op	      32 B/op	       1 allocs/op
GencodeUnmarshal               	 3000000	       455 ns/op	     115 B/op	       7 allocs/op
GencodeUnsafeMarshal           	10000000	       214 ns/op	      32 B/op	       1 allocs/op
GencodeUnsafeUnmarshal         	 3000000	       455 ns/op	     115 B/op	       7 allocs/op
XDR2Marshal                    	 5000000	       372 ns/op	      48 B/op	       1 allocs/op
XDR2Unmarshal                  	 3000000	       519 ns/op	      19 B/op	       6 allocs/op
GoAvroMarshal                  	  300000	      4757 ns/op	    1104 B/op	      37 allocs/op
GoAvroUnmarshal                	  200000	     11065 ns/op	    3472 B/op	     101 allocs/op
GoAvro2TextMarshal             	  300000	      4598 ns/op	    1112 B/op	      21 allocs/op
GoAvro2TextUnmarshal           	  300000	      4680 ns/op	     816 B/op	      39 allocs/op
GoAvro2BinaryMarshal           	 1000000	      2115 ns/op	     536 B/op	      17 allocs/op
GoAvro2BinaryUnmarshal         	  500000	      2965 ns/op	     816 B/op	      27 allocs/op
PASS
ok  	github.com/alecthomas/go_serialization_s	86.764s

MsgpMarshal                    	 5000000	       299 ns/op	      96 B/op	       1 allocs/op
MsgpUnmarshal                  	 2000000	       817 ns/op	     115 B/op	       7 allocs/op
VmihailencoMsgpackMarshal      	  500000	      3385 ns/op	     336 B/op	       5 allocs/op
VmihailencoMsgpackUnmarshal    	  500000	      3766 ns/op	     352 B/op	      16 allocs/op
JsonMarshal                    	  500000	      2848 ns/op	     512 B/op	       6 allocs/op
JsonUnmarshal                  	  500000	      3525 ns/op	     424 B/op	      10 allocs/op
EasyJsonMarshal                	 2000000	       655 ns/op	     128 B/op	       1 allocs/op
EasyJsonUnmarshal              	 1000000	      1208 ns/op	     115 B/op	       7 allocs/op
BsonMarshal                    	  500000	      2676 ns/op	     384 B/op	       9 allocs/op
BsonUnmarshal                  	  500000	      3791 ns/op	     272 B/op	      25 allocs/op
GobMarshal                     	 1000000	      1299 ns/op	       0 B/op	       0 allocs/op
GobUnmarshal                   	 1000000	      1718 ns/op	     115 B/op	       7 allocs/op
XdrMarshal                     	 1000000	      2184 ns/op	     224 B/op	      22 allocs/op
XdrUnmarshal                   	 1000000	      1822 ns/op	     192 B/op	      13 allocs/op
UgorjiCodecMsgpackMarshal      	 1000000	      2237 ns/op	    1360 B/op	       5 allocs/op
UgorjiCodecMsgpackUnmarshal    	  500000	      2844 ns/op	    1347 B/op	       9 allocs/op
UgorjiCodecBincMarshal         	 1000000	      2267 ns/op	    1392 B/op	       5 allocs/op
UgorjiCodecBincUnmarshal       	  500000	      2833 ns/op	    1379 B/op	       9 allocs/op
SerealMarshal                  	  300000	      4496 ns/op	     800 B/op	      17 allocs/op
SerealUnmarshal                	  300000	      4767 ns/op	     824 B/op	      30 allocs/op
BinaryMarshal                  	 1000000	      2262 ns/op	     216 B/op	      14 allocs/op
BinaryUnmarshal                	  500000	      2597 ns/op	     256 B/op	      27 allocs/op
FlatBuffersMarshal             	 2000000	       702 ns/op	       0 B/op	       0 allocs/op
FlatBuffersUnmarshal           	 2000000	       710 ns/op	     115 B/op	       7 allocs/op
CapNProtoMarshal               	 1000000	      1279 ns/op	      56 B/op	       2 allocs/op
CapNProtoUnmarshal             	 1000000	      1156 ns/op	     203 B/op	      10 allocs/op
HproseMarshal                  	 1000000	      1214 ns/op	     233 B/op	       6 allocs/op
HproseUnmarshal                	  500000	      2435 ns/op	     320 B/op	      20 allocs/op
ProtobufMarshal                	 1000000	      1882 ns/op	     208 B/op	       7 allocs/op
ProtobufUnmarshal              	 1000000	      1284 ns/op	     192 B/op	      13 allocs/op
GoprotobufMarshal              	 2000000	       878 ns/op	     264 B/op	       4 allocs/op
GoprotobufUnmarshal            	 1000000	      1398 ns/op	     403 B/op	      14 allocs/op
GogoprotobufMarshal            	 5000000	       265 ns/op	      32 B/op	       1 allocs/op
GogoprotobufUnmarshal          	 3000000	       548 ns/op	     115 B/op	       7 allocs/op
ColferMarshal                  	10000000	       220 ns/op	      32 B/op	       1 allocs/op
ColferUnmarshal                	 3000000	       472 ns/op	     115 B/op	       7 allocs/op
GencodeMarshal                 	10000000	       211 ns/op	      32 B/op	       1 allocs/op
GencodeUnmarshal               	 3000000	       452 ns/op	     115 B/op	       7 allocs/op
GencodeUnsafeMarshal           	10000000	       212 ns/op	      32 B/op	       1 allocs/op
GencodeUnsafeUnmarshal         	 3000000	       452 ns/op	     115 B/op	       7 allocs/op
XDR2Marshal                    	 5000000	       370 ns/op	      48 B/op	       1 allocs/op
XDR2Unmarshal                  	 3000000	       522 ns/op	      19 B/op	       6 allocs/op
GoAvroMarshal                  	  300000	      4765 ns/op	    1104 B/op	      37 allocs/op
GoAvroUnmarshal                	  200000	     11041 ns/op	    3472 B/op	     101 allocs/op
GoAvro2TextMarshal             	  300000	      4623 ns/op	    1112 B/op	      21 allocs/op
GoAvro2TextUnmarshal           	  300000	      4725 ns/op	     816 B/op	      39 allocs/op
GoAvro2BinaryMarshal           	 1000000	      2114 ns/op	     536 B/op	      17 allocs/op
GoAvro2BinaryUnmarshal         	  500000	      3001 ns/op	     816 B/op	      27 allocs/op
PASS
ok  	github.com/alecthomas/go_serialization_s	86.605s

MsgpMarshal                    	 5000000	       294 ns/op	      96 B/op	       1 allocs/op
MsgpUnmarshal                  	 2000000	       836 ns/op	     115 B/op	       7 allocs/op
VmihailencoMsgpackMarshal      	  500000	      3396 ns/op	     336 B/op	       5 allocs/op
VmihailencoMsgpackUnmarshal    	  500000	      3791 ns/op	     352 B/op	      16 allocs/op
JsonMarshal                    	  500000	      2838 ns/op	     512 B/op	       6 allocs/op
JsonUnmarshal                  	  500000	      3501 ns/op	     424 B/op	      10 allocs/op
EasyJsonMarshal                	 2000000	       649 ns/op	     128 B/op	       1 allocs/op
EasyJsonUnmarshal              	 1000000	      1212 ns/op	     115 B/op	       7 allocs/op
BsonMarshal                    	  500000	      2672 ns/op	     384 B/op	       9 allocs/op
BsonUnmarshal                  	  500000	      3786 ns/op	     272 B/op	      25 allocs/op
GobMarshal                     	 1000000	      1318 ns/op	       0 B/op	       0 allocs/op
GobUnmarshal                   	 1000000	      1746 ns/op	     115 B/op	       7 allocs/op
XdrMarshal                     	 1000000	      2200 ns/op	     224 B/op	      22 allocs/op
XdrUnmarshal                   	 1000000	      1803 ns/op	     192 B/op	      13 allocs/op
UgorjiCodecMsgpackMarshal      	 1000000	      2230 ns/op	    1360 B/op	       5 allocs/op
UgorjiCodecMsgpackUnmarshal    	  500000	      2873 ns/op	    1347 B/op	       9 allocs/op
UgorjiCodecBincMarshal         	  500000	      2294 ns/op	    1392 B/op	       5 allocs/op
UgorjiCodecBincUnmarshal       	  500000	      2823 ns/op	    1379 B/op	       9 allocs/op
SerealMarshal                  	  300000	      4454 ns/op	     800 B/op	      17 allocs/op
SerealUnmarshal                	  300000	      4827 ns/op	     824 B/op	      30 allocs/op
BinaryMarshal                  	 1000000	      2249 ns/op	     216 B/op	      14 allocs/op
BinaryUnmarshal                	  500000	      2578 ns/op	     256 B/op	      27 allocs/op
FlatBuffersMarshal             	 2000000	       699 ns/op	       0 B/op	       0 allocs/op
FlatBuffersUnmarshal           	 2000000	       707 ns/op	     115 B/op	       7 allocs/op
CapNProtoMarshal               	 1000000	      1276 ns/op	      56 B/op	       2 allocs/op
CapNProtoUnmarshal             	 1000000	      1129 ns/op	     203 B/op	      10 allocs/op
HproseMarshal                  	 1000000	      1235 ns/op	     233 B/op	       6 allocs/op
HproseUnmarshal                	  500000	      2467 ns/op	     320 B/op	      20 allocs/op
ProtobufMarshal                	 1000000	      1863 ns/op	     208 B/op	       7 allocs/op
ProtobufUnmarshal              	 1000000	      1269 ns/op	     192 B/op	      13 allocs/op
GoprotobufMarshal              	 2000000	       864 ns/op	     264 B/op	       4 allocs/op
GoprotobufUnmarshal            	 1000000	      1400 ns/op	     403 B/op	      14 allocs/op
GogoprotobufMarshal            	 5000000	       270 ns/op	      32 B/op	       1 allocs/op
GogoprotobufUnmarshal          	 3000000	       561 ns/op	     115 B/op	       7 allocs/op
ColferMarshal                  	10000000	       223 ns/op	      32 B/op	       1 allocs/op
ColferUnmarshal                	 3000000	       481 ns/op	     115 B/op	       7 allocs/op
GencodeMarshal                 	10000000	       211 ns/op	      32 B/op	       1 allocs/op
GencodeUnmarshal               	 3000000	       454 ns/op	     115 B/op	       7 allocs/op
GencodeUnsafeMarshal           	10000000	       213 ns/op	      32 B/op	       1 allocs/op
GencodeUnsafeUnmarshal         	 3000000	       449 ns/op	     115 B/op	       7 allocs/op
XDR2Marshal                    	 5000000	       372 ns/op	      48 B/op	       1 allocs/op
XDR2Unmarshal                  	 3000000	       520 ns/op	      19 B/op	       6 allocs/op
GoAvroMarshal                  	  300000	      4743 ns/op	    1104 B/op	      37 allocs/op
GoAvroUnmarshal                	  200000	     11061 ns/op	    3472 B/op	     101 allocs/op
GoAvro2TextMarshal             	  300000	      4597 ns/op	    1112 B/op	      21 allocs/op
GoAvro2TextUnmarshal           	  300000	      4716 ns/op	     816 B/op	      39 allocs/op
GoAvro2BinaryMarshal           	 1000000	      2095 ns/op	     536 B/op	      17 allocs/op
GoAvro2BinaryUnmarshal         	  500000	      2988 ns/op	     816 B/op	      27 allocs/op
PASS
ok  	github.com/alecthomas/go_serialization_s	85.559s

