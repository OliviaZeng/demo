BenchmarkXDR2Unmarshal-12                  	 5000000	       286 ns/op	      32 B/op	       2 allocs/op
BenchmarkGencodeUnsafeUnmarshal-12         	 5000000	       282 ns/op	      96 B/op	       3 allocs/op
BenchmarkFlatBuffersUnmarshal-12           	 3000000	       493 ns/op	     112 B/op	       3 allocs/op
BenchmarkGobUnmarshal-12                   	 1000000	      1718 ns/op	     112 B/op	       3 allocs/op
BenchmarkGogoprotobufUnmarshal-12          	 3000000	       562 ns/op	     112 B/op	       3 allocs/op
BenchmarkMsgpUnmarshal-12                  	 2000000	       641 ns/op	     112 B/op	       3 allocs/op
BenchmarkColferUnmarshal-12                	 3000000	       455 ns/op	     144 B/op	       4 allocs/op
BenchmarkGencodeUnmarshal-12               	 5000000	       376 ns/op	     144 B/op	       3 allocs/op
BenchmarkEasyJsonUnmarshal-12              	  500000	      2468 ns/op	     160 B/op	       4 allocs/op
BenchmarkProtobufUnmarshal-12              	 1000000	      1345 ns/op	     192 B/op	      10 allocs/op
BenchmarkCapNProtoUnmarshal-12             	 2000000	       789 ns/op	     200 B/op	       6 allocs/op
BenchmarkXdrUnmarshal-12                   	  500000	      2631 ns/op	     240 B/op	      11 allocs/op
BenchmarkBsonUnmarshal-12                  	  500000	      3448 ns/op	     248 B/op	      21 allocs/op
BenchmarkHproseUnmarshal-12                	 1000000	      1933 ns/op	     320 B/op	      10 allocs/op
BenchmarkBinaryUnmarshal-12                	  500000	      2754 ns/op	     336 B/op	      22 allocs/op
BenchmarkVmihailencoMsgpackUnmarshal-12    	  500000	      3761 ns/op	     384 B/op	      13 allocs/op
BenchmarkGoprotobufUnmarshal-12            	 1000000	      1199 ns/op	     432 B/op	       9 allocs/op
BenchmarkJsonUnmarshal-12                  	  300000	      5748 ns/op	     464 B/op	       7 allocs/op
BenchmarkGoAvro2BinaryUnmarshal-12         	 1000000	      2105 ns/op	     584 B/op	      15 allocs/op
BenchmarkGoAvro2TextUnmarshal-12           	  300000	      5456 ns/op	     807 B/op	      36 allocs/op
BenchmarkSerealUnmarshal-12                	  300000	      5836 ns/op	    1008 B/op	      34 allocs/op
BenchmarkUgorjiCodecMsgpackUnmarshal-12    	  500000	      2948 ns/op	    1344 B/op	       5 allocs/op
BenchmarkUgorjiCodecBincUnmarshal-12       	  500000	      3440 ns/op	    1520 B/op	       8 allocs/op
BenchmarkGoAvroUnmarshal-12                	  200000	     10818 ns/op	    3440 B/op	      89 allocs/op
