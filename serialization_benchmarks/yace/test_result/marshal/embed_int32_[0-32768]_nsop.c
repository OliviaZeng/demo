BenchmarkGencodeUnsafeMarshal-12           	10000000	       196 ns/op	      48 B/op	       1 allocs/op
BenchmarkColferMarshal-12                  	 5000000	       288 ns/op	      80 B/op	       1 allocs/op
BenchmarkXDR2Marshal-12                    	 5000000	       304 ns/op	      64 B/op	       1 allocs/op
BenchmarkMsgpMarshal-12                    	 5000000	       319 ns/op	     128 B/op	       1 allocs/op
BenchmarkGencodeMarshal-12                 	 5000000	       344 ns/op	      96 B/op	       2 allocs/op
BenchmarkGogoprotobufMarshal-12            	 5000000	       378 ns/op	      80 B/op	       1 allocs/op
BenchmarkFlatBuffersMarshal-12             	 2000000	       622 ns/op	       0 B/op	       0 allocs/op
BenchmarkCapNProtoMarshal-12               	 2000000	       852 ns/op	      56 B/op	       2 allocs/op
BenchmarkGoprotobufMarshal-12              	 2000000	       929 ns/op	     312 B/op	       4 allocs/op
BenchmarkGobMarshal-12                     	 1000000	      1680 ns/op	      48 B/op	       2 allocs/op
BenchmarkProtobufMarshal-12                	 1000000	      1701 ns/op	     200 B/op	       7 allocs/op
BenchmarkHproseMarshal-12                  	 1000000	      1737 ns/op	     473 B/op	       8 allocs/op
BenchmarkGoAvro2BinaryMarshal-12           	 1000000	      1932 ns/op	     512 B/op	      13 allocs/op
BenchmarkBinaryMarshal-12                  	 1000000	      2383 ns/op	     256 B/op	      16 allocs/op
BenchmarkEasyJsonMarshal-12                	  500000	      2396 ns/op	     784 B/op	       5 allocs/op
BenchmarkBsonMarshal-12                    	  500000	      2466 ns/op	     392 B/op	      10 allocs/op
BenchmarkUgorjiCodecBincMarshal-12         	  500000	      2901 ns/op	    1408 B/op	       6 allocs/op
BenchmarkUgorjiCodecMsgpackMarshal-12      	  500000	      2920 ns/op	    1376 B/op	       6 allocs/op
BenchmarkXdrMarshal-12                     	  500000	      2987 ns/op	     456 B/op	      21 allocs/op
BenchmarkVmihailencoMsgpackMarshal-12      	  500000	      3574 ns/op	     368 B/op	       6 allocs/op
BenchmarkGoAvroMarshal-12                  	  300000	      4671 ns/op	    1032 B/op	      33 allocs/op
BenchmarkSerealMarshal-12                  	  300000	      5566 ns/op	     912 B/op	      21 allocs/op
BenchmarkGoAvro2TextMarshal-12             	  300000	      5637 ns/op	    1328 B/op	      22 allocs/op
BenchmarkJsonMarshal-12                    	  300000	      5651 ns/op	    1232 B/op	      10 allocs/op
