BenchmarkMsgpMarshal-12                    	 5000000	       318 ns/op	     128 B/op	       1 allocs/op
BenchmarkMsgpUnmarshal-12                  	 2000000	       641 ns/op	     112 B/op	       3 allocs/op
BenchmarkVmihailencoMsgpackMarshal-12      	  500000	      3574 ns/op	     368 B/op	       6 allocs/op
BenchmarkVmihailencoMsgpackUnmarshal-12    	  500000	      3760 ns/op	     384 B/op	      13 allocs/op
BenchmarkJsonMarshal-12                    	  300000	      5644 ns/op	    1232 B/op	      10 allocs/op
BenchmarkJsonUnmarshal-12                  	  300000	      5735 ns/op	     464 B/op	       7 allocs/op
BenchmarkEasyJsonMarshal-12                	 1000000	      2403 ns/op	     784 B/op	       5 allocs/op
BenchmarkEasyJsonUnmarshal-12              	  500000	      2459 ns/op	     160 B/op	       4 allocs/op
BenchmarkBsonMarshal-12                    	  500000	      2479 ns/op	     392 B/op	      10 allocs/op
BenchmarkBsonUnmarshal-12                  	  500000	      3428 ns/op	     248 B/op	      21 allocs/op
BenchmarkGobMarshal-12                     	 1000000	      1685 ns/op	      48 B/op	       2 allocs/op
BenchmarkGobUnmarshal-12                   	 1000000	      1694 ns/op	     112 B/op	       3 allocs/op
BenchmarkXdrMarshal-12                     	  500000	      2969 ns/op	     455 B/op	      20 allocs/op
BenchmarkXdrUnmarshal-12                   	  500000	      2615 ns/op	     239 B/op	      11 allocs/op
BenchmarkUgorjiCodecMsgpackMarshal-12      	  500000	      2889 ns/op	    1376 B/op	       6 allocs/op
BenchmarkUgorjiCodecMsgpackUnmarshal-12    	  500000	      2959 ns/op	    1344 B/op	       5 allocs/op
BenchmarkUgorjiCodecBincMarshal-12         	  500000	      2786 ns/op	    1408 B/op	       6 allocs/op
BenchmarkUgorjiCodecBincUnmarshal-12       	  500000	      3435 ns/op	    1520 B/op	       8 allocs/op
BenchmarkSerealMarshal-12                  	  300000	      5529 ns/op	     912 B/op	      21 allocs/op
BenchmarkSerealUnmarshal-12                	  300000	      5832 ns/op	    1008 B/op	      34 allocs/op
BenchmarkBinaryMarshal-12                  	 1000000	      2389 ns/op	     256 B/op	      16 allocs/op
BenchmarkBinaryUnmarshal-12                	  500000	      2750 ns/op	     336 B/op	      22 allocs/op
BenchmarkFlatBuffersMarshal-12             	 2000000	       623 ns/op	       0 B/op	       0 allocs/op
BenchmarkFlatBuffersUnmarshal-12           	 3000000	       490 ns/op	     112 B/op	       3 allocs/op
BenchmarkCapNProtoMarshal-12               	 2000000	       850 ns/op	      56 B/op	       2 allocs/op
BenchmarkCapNProtoUnmarshal-12             	 2000000	       790 ns/op	     200 B/op	       6 allocs/op
BenchmarkHproseMarshal-12                  	 1000000	      1745 ns/op	     472 B/op	       8 allocs/op
BenchmarkHproseUnmarshal-12                	 1000000	      1921 ns/op	     319 B/op	      10 allocs/op
BenchmarkProtobufMarshal-12                	 1000000	      1695 ns/op	     200 B/op	       7 allocs/op
BenchmarkProtobufUnmarshal-12              	 1000000	      1343 ns/op	     192 B/op	      10 allocs/op
BenchmarkGoprotobufMarshal-12              	 2000000	       921 ns/op	     312 B/op	       4 allocs/op
BenchmarkGoprotobufUnmarshal-12            	 1000000	      1195 ns/op	     432 B/op	       9 allocs/op
BenchmarkGogoprotobufMarshal-12            	 5000000	       360 ns/op	      80 B/op	       1 allocs/op
BenchmarkGogoprotobufUnmarshal-12          	 3000000	       537 ns/op	     112 B/op	       3 allocs/op
BenchmarkColferMarshal-12                  	 5000000	       268 ns/op	      64 B/op	       1 allocs/op
BenchmarkColferUnmarshal-12                	 3000000	       429 ns/op	     144 B/op	       4 allocs/op
BenchmarkGencodeMarshal-12                 	 5000000	       340 ns/op	      96 B/op	       2 allocs/op
BenchmarkGencodeUnmarshal-12               	 5000000	       376 ns/op	     144 B/op	       3 allocs/op
BenchmarkGencodeUnsafeMarshal-12           	10000000	       198 ns/op	      48 B/op	       1 allocs/op
BenchmarkGencodeUnsafeUnmarshal-12         	 5000000	       285 ns/op	      96 B/op	       3 allocs/op
BenchmarkXDR2Marshal-12                    	 5000000	       307 ns/op	      64 B/op	       1 allocs/op
BenchmarkXDR2Unmarshal-12                  	 5000000	       288 ns/op	      32 B/op	       2 allocs/op
BenchmarkGoAvroMarshal-12                  	  300000	      4668 ns/op	    1032 B/op	      33 allocs/op
BenchmarkGoAvroUnmarshal-12                	  200000	     10869 ns/op	    3440 B/op	      89 allocs/op
BenchmarkGoAvro2TextMarshal-12             	  300000	      5628 ns/op	    1328 B/op	      22 allocs/op
BenchmarkGoAvro2TextUnmarshal-12           	  300000	      5461 ns/op	     807 B/op	      36 allocs/op
BenchmarkGoAvro2BinaryMarshal-12           	 1000000	      1932 ns/op	     512 B/op	      13 allocs/op
BenchmarkGoAvro2BinaryUnmarshal-12         	 1000000	      2120 ns/op	     584 B/op	      15 allocs/op
PASS
ok  	github.com/alecthomas/go_serialization_benchmarks	88.624s

BenchmarkMsgpMarshal-12                    	 5000000	       320 ns/op	     128 B/op	       1 allocs/op
BenchmarkMsgpUnmarshal-12                  	 2000000	       641 ns/op	     112 B/op	       3 allocs/op
BenchmarkVmihailencoMsgpackMarshal-12      	  500000	      3565 ns/op	     368 B/op	       6 allocs/op
BenchmarkVmihailencoMsgpackUnmarshal-12    	  500000	      3770 ns/op	     384 B/op	      13 allocs/op
BenchmarkJsonMarshal-12                    	  200000	      5693 ns/op	    1232 B/op	      10 allocs/op
BenchmarkJsonUnmarshal-12                  	  300000	      5746 ns/op	     464 B/op	       7 allocs/op
BenchmarkEasyJsonMarshal-12                	  500000	      2398 ns/op	     784 B/op	       5 allocs/op
BenchmarkEasyJsonUnmarshal-12              	  500000	      2469 ns/op	     160 B/op	       4 allocs/op
BenchmarkBsonMarshal-12                    	  500000	      2461 ns/op	     392 B/op	      10 allocs/op
BenchmarkBsonUnmarshal-12                  	  500000	      3431 ns/op	     248 B/op	      21 allocs/op
BenchmarkGobMarshal-12                     	 1000000	      1666 ns/op	      48 B/op	       2 allocs/op
BenchmarkGobUnmarshal-12                   	 1000000	      1699 ns/op	     112 B/op	       3 allocs/op
BenchmarkXdrMarshal-12                     	  500000	      2952 ns/op	     455 B/op	      20 allocs/op
BenchmarkXdrUnmarshal-12                   	  500000	      2616 ns/op	     240 B/op	      11 allocs/op
BenchmarkUgorjiCodecMsgpackMarshal-12      	  500000	      2870 ns/op	    1376 B/op	       6 allocs/op
BenchmarkUgorjiCodecMsgpackUnmarshal-12    	  500000	      2971 ns/op	    1344 B/op	       5 allocs/op
BenchmarkUgorjiCodecBincMarshal-12         	  500000	      2824 ns/op	    1408 B/op	       6 allocs/op
BenchmarkUgorjiCodecBincUnmarshal-12       	  500000	      3453 ns/op	    1520 B/op	       8 allocs/op
BenchmarkSerealMarshal-12                  	  300000	      5589 ns/op	     912 B/op	      21 allocs/op
BenchmarkSerealUnmarshal-12                	  300000	      5863 ns/op	    1008 B/op	      34 allocs/op
BenchmarkBinaryMarshal-12                  	 1000000	      2365 ns/op	     256 B/op	      16 allocs/op
BenchmarkBinaryUnmarshal-12                	  500000	      2738 ns/op	     336 B/op	      22 allocs/op
BenchmarkFlatBuffersMarshal-12             	 2000000	       622 ns/op	       0 B/op	       0 allocs/op
BenchmarkFlatBuffersUnmarshal-12           	 3000000	       496 ns/op	     112 B/op	       3 allocs/op
BenchmarkCapNProtoMarshal-12               	 2000000	       850 ns/op	      56 B/op	       2 allocs/op
BenchmarkCapNProtoUnmarshal-12             	 2000000	       785 ns/op	     200 B/op	       6 allocs/op
BenchmarkHproseMarshal-12                  	 1000000	      1739 ns/op	     478 B/op	       8 allocs/op
BenchmarkHproseUnmarshal-12                	 1000000	      1950 ns/op	     320 B/op	      10 allocs/op
BenchmarkProtobufMarshal-12                	 1000000	      1697 ns/op	     200 B/op	       7 allocs/op
BenchmarkProtobufUnmarshal-12              	 1000000	      1341 ns/op	     192 B/op	      10 allocs/op
BenchmarkGoprotobufMarshal-12              	 2000000	       923 ns/op	     312 B/op	       4 allocs/op
BenchmarkGoprotobufUnmarshal-12            	 1000000	      1189 ns/op	     432 B/op	       9 allocs/op
BenchmarkGogoprotobufMarshal-12            	 5000000	       361 ns/op	      80 B/op	       1 allocs/op
BenchmarkGogoprotobufUnmarshal-12          	 3000000	       537 ns/op	     112 B/op	       3 allocs/op
BenchmarkColferMarshal-12                  	 5000000	       267 ns/op	      64 B/op	       1 allocs/op
BenchmarkColferUnmarshal-12                	 3000000	       433 ns/op	     144 B/op	       4 allocs/op
BenchmarkGencodeMarshal-12                 	 5000000	       343 ns/op	      96 B/op	       2 allocs/op
BenchmarkGencodeUnmarshal-12               	 5000000	       377 ns/op	     144 B/op	       3 allocs/op
BenchmarkGencodeUnsafeMarshal-12           	10000000	       200 ns/op	      48 B/op	       1 allocs/op
BenchmarkGencodeUnsafeUnmarshal-12         	 5000000	       283 ns/op	      96 B/op	       3 allocs/op
BenchmarkXDR2Marshal-12                    	 5000000	       309 ns/op	      64 B/op	       1 allocs/op
BenchmarkXDR2Unmarshal-12                  	 5000000	       287 ns/op	      32 B/op	       2 allocs/op
BenchmarkGoAvroMarshal-12                  	  300000	      4654 ns/op	    1032 B/op	      33 allocs/op
BenchmarkGoAvroUnmarshal-12                	  200000	     10891 ns/op	    3440 B/op	      89 allocs/op
BenchmarkGoAvro2TextMarshal-12             	  300000	      5631 ns/op	    1328 B/op	      22 allocs/op
BenchmarkGoAvro2TextUnmarshal-12           	  300000	      5462 ns/op	     808 B/op	      36 allocs/op
BenchmarkGoAvro2BinaryMarshal-12           	 1000000	      1929 ns/op	     512 B/op	      13 allocs/op
BenchmarkGoAvro2BinaryUnmarshal-12         	 1000000	      2099 ns/op	     584 B/op	      15 allocs/op
PASS
ok  	github.com/alecthomas/go_serialization_benchmarks	86.951s

BenchmarkMsgpMarshal-12                    	 5000000	       316 ns/op	     128 B/op	       1 allocs/op
BenchmarkMsgpUnmarshal-12                  	 2000000	       634 ns/op	     112 B/op	       3 allocs/op
BenchmarkVmihailencoMsgpackMarshal-12      	  500000	      3580 ns/op	     368 B/op	       6 allocs/op
BenchmarkVmihailencoMsgpackUnmarshal-12    	  500000	      3760 ns/op	     384 B/op	      13 allocs/op
BenchmarkJsonMarshal-12                    	  300000	      5667 ns/op	    1232 B/op	      10 allocs/op
BenchmarkJsonUnmarshal-12                  	  300000	      5744 ns/op	     464 B/op	       7 allocs/op
BenchmarkEasyJsonMarshal-12                	  500000	      2422 ns/op	     784 B/op	       5 allocs/op
BenchmarkEasyJsonUnmarshal-12              	  500000	      2474 ns/op	     160 B/op	       4 allocs/op
BenchmarkBsonMarshal-12                    	  500000	      2456 ns/op	     392 B/op	      10 allocs/op
BenchmarkBsonUnmarshal-12                  	  500000	      3447 ns/op	     248 B/op	      21 allocs/op
BenchmarkGobMarshal-12                     	 1000000	      1668 ns/op	      48 B/op	       2 allocs/op
BenchmarkGobUnmarshal-12                   	 1000000	      1731 ns/op	     112 B/op	       3 allocs/op
BenchmarkXdrMarshal-12                     	  500000	      2980 ns/op	     455 B/op	      20 allocs/op
BenchmarkXdrUnmarshal-12                   	  500000	      2613 ns/op	     240 B/op	      11 allocs/op
BenchmarkUgorjiCodecMsgpackMarshal-12      	  500000	      2859 ns/op	    1376 B/op	       6 allocs/op
BenchmarkUgorjiCodecMsgpackUnmarshal-12    	  500000	      2954 ns/op	    1344 B/op	       5 allocs/op
BenchmarkUgorjiCodecBincMarshal-12         	  500000	      2823 ns/op	    1408 B/op	       6 allocs/op
BenchmarkUgorjiCodecBincUnmarshal-12       	  500000	      3420 ns/op	    1520 B/op	       8 allocs/op
BenchmarkSerealMarshal-12                  	  300000	      5519 ns/op	     912 B/op	      21 allocs/op
BenchmarkSerealUnmarshal-12                	  300000	      5830 ns/op	    1008 B/op	      34 allocs/op
BenchmarkBinaryMarshal-12                  	 1000000	      2368 ns/op	     256 B/op	      16 allocs/op
BenchmarkBinaryUnmarshal-12                	  500000	      2742 ns/op	     336 B/op	      22 allocs/op
BenchmarkFlatBuffersMarshal-12             	 2000000	       628 ns/op	       0 B/op	       0 allocs/op
BenchmarkFlatBuffersUnmarshal-12           	 3000000	       493 ns/op	     112 B/op	       3 allocs/op
BenchmarkCapNProtoMarshal-12               	 2000000	       847 ns/op	      56 B/op	       2 allocs/op
BenchmarkCapNProtoUnmarshal-12             	 2000000	       781 ns/op	     200 B/op	       6 allocs/op
BenchmarkHproseMarshal-12                  	 1000000	      1741 ns/op	     473 B/op	       8 allocs/op
BenchmarkHproseUnmarshal-12                	 1000000	      1945 ns/op	     320 B/op	      10 allocs/op
BenchmarkProtobufMarshal-12                	 1000000	      1684 ns/op	     200 B/op	       7 allocs/op
BenchmarkProtobufUnmarshal-12              	 1000000	      1343 ns/op	     192 B/op	      10 allocs/op
BenchmarkGoprotobufMarshal-12              	 2000000	       910 ns/op	     312 B/op	       4 allocs/op
BenchmarkGoprotobufUnmarshal-12            	 1000000	      1191 ns/op	     432 B/op	       9 allocs/op
BenchmarkGogoprotobufMarshal-12            	 5000000	       360 ns/op	      80 B/op	       1 allocs/op
BenchmarkGogoprotobufUnmarshal-12          	 3000000	       535 ns/op	     112 B/op	       3 allocs/op
BenchmarkColferMarshal-12                  	 5000000	       273 ns/op	      64 B/op	       1 allocs/op
BenchmarkColferUnmarshal-12                	 3000000	       428 ns/op	     144 B/op	       4 allocs/op
BenchmarkGencodeMarshal-12                 	 5000000	       346 ns/op	      96 B/op	       2 allocs/op
BenchmarkGencodeUnmarshal-12               	 5000000	       380 ns/op	     144 B/op	       3 allocs/op
BenchmarkGencodeUnsafeMarshal-12           	10000000	       199 ns/op	      48 B/op	       1 allocs/op
BenchmarkGencodeUnsafeUnmarshal-12         	 5000000	       281 ns/op	      96 B/op	       3 allocs/op
BenchmarkXDR2Marshal-12                    	 5000000	       310 ns/op	      64 B/op	       1 allocs/op
BenchmarkXDR2Unmarshal-12                  	 5000000	       286 ns/op	      32 B/op	       2 allocs/op
BenchmarkGoAvroMarshal-12                  	  300000	      4654 ns/op	    1032 B/op	      33 allocs/op
BenchmarkGoAvroUnmarshal-12                	  200000	     10861 ns/op	    3440 B/op	      89 allocs/op
BenchmarkGoAvro2TextMarshal-12             	  300000	      5626 ns/op	    1328 B/op	      22 allocs/op
BenchmarkGoAvro2TextUnmarshal-12           	  300000	      5422 ns/op	     807 B/op	      36 allocs/op
BenchmarkGoAvro2BinaryMarshal-12           	 1000000	      1922 ns/op	     512 B/op	      13 allocs/op
BenchmarkGoAvro2BinaryUnmarshal-12         	 1000000	      2086 ns/op	     584 B/op	      15 allocs/op
PASS
ok  	github.com/alecthomas/go_serialization_benchmarks	87.414s

