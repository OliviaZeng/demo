head -48 bool_\[0-1\].c | grep Marshal | sort -n -k 3 > marshal/bool_\[0-1\]_nsop.c
head -48 bool_\[0-1\].c | grep Marshal | sort -n -k 5 > marshal/bool_\[0-1\]_Bop.c

head -48 double.c | grep Marshal | sort -n -k 3 > marshal/double_nsop.c
head -48 double.c | grep Marshal | sort -n -k 5 > marshal/double_Bop.c

head -48 int32_\[0-32768\].c | grep Marshal | sort -n -k 3 > marshal/int32_\[0-32768\]_nsop.c
head -48 int32_\[0-32768\].c | grep Marshal | sort -n -k 5 > marshal/int32_\[0-32768\]_Bop.c

head -48 int32_32768.c | grep Marshal | sort -n -k 3 > marshal/int32_32768_nsop.c
head -48 int32_32768.c | grep Marshal | sort -n -k 5 > marshal/int32_32768_Bop.c

head -48 int32_-5.c | grep Marshal | sort -n -k 3 > marshal/int32_-5_nsop.c
head -48 int32_-5.c | grep Marshal | sort -n -k 5 > marshal/int32_-5_Bop.c

head -48 int32_5.c | grep Marshal | sort -n -k 3 > marshal/int32_5_nsop.c
head -48 int32_5.c | grep Marshal | sort -n -k 5 > marshal/int32_5_Bop.c

head -48 ori.c | grep Marshal | sort -n -k 3 > marshal/ori_nsop.c
head -48 ori.c | grep Marshal | sort -n -k 5 > marshal/ori_Bop.c

head -48 string_16.c | grep Marshal | sort -n -k 3 > marshal/string_16_nsop.c
head -48 string_16.c | grep Marshal | sort -n -k 5 > marshal/string_16_Bop.c

head -48 string_3.c | grep Marshal | sort -n -k 3 > marshal/string_3_nsop.c
head -48 string_3.c | grep Marshal | sort -n -k 5 > marshal/string_3_Bop.c

head -48 embed_int32_\[0-5\].c | grep Marshal | sort -n -k 3 > marshal/embed_int32_\[0-5\]_nsop.c
head -48 embed_int32_\[0-5\].c | grep Marshal | sort -n -k 5 > marshal/embed_int32_\[0-5\]_Bop.c

head -48 embed_int32_\[0-32768\].c | grep Marshal | sort -n -k 3 > marshal/embed_int32_\[0-32768\]_nsop.c
head -48 embed_int32_\[0-32768\].c | grep Marshal | sort -n -k 5 > marshal/embed_int32_\[0-32768\]_Bop.c

head -48 embed_string_3.c | grep Marshal | sort -n -k 3 > marshal/embed_string_3_nsop.c
head -48 embed_string_3.c | grep Marshal | sort -n -k 5 > marshal/embed_string_3_Bop.c

#unmarshal
head -48 bool_\[0-1\].c | grep Unmarshal | sort -n -k 3 > unmarshal/bool_\[0-1\]_nsop.c
head -48 bool_\[0-1\].c | grep Unmarshal | sort -n -k 5 > unmarshal/bool_\[0-1\]_Bop.c

head -48 double.c | grep Unmarshal | sort -n -k 3 > unmarshal/double_nsop.c
head -48 double.c | grep Unmarshal | sort -n -k 5 > unmarshal/double_Bop.c

head -48 int32_\[0-32768\].c | grep Unmarshal | sort -n -k 3 > unmarshal/int32_\[0-32768\]_nsop.c
head -48 int32_\[0-32768\].c | grep Unmarshal | sort -n -k 5 > unmarshal/int32_\[0-32768\]_Bop.c

head -48 int32_32768.c | grep Unmarshal | sort -n -k 3 > unmarshal/int32_32768_nsop.c
head -48 int32_32768.c | grep Unmarshal | sort -n -k 5 > unmarshal/int32_32768_Bop.c

head -48 int32_-5.c | grep Unmarshal | sort -n -k 3 > unmarshal/int32_-5_nsop.c
head -48 int32_-5.c | grep Unmarshal | sort -n -k 5 > unmarshal/int32_-5_Bop.c

head -48 int32_5.c | grep Unmarshal | sort -n -k 3 > unmarshal/int32_5_nsop.c
head -48 int32_5.c | grep Unmarshal | sort -n -k 5 > unmarshal/int32_5_Bop.c

head -48 ori.c | grep Unmarshal | sort -n -k 3 > unmarshal/ori_nsop.c
head -48 ori.c | grep Unmarshal | sort -n -k 5 > unmarshal/ori_Bop.c

head -48 string_16.c | grep Unmarshal | sort -n -k 3 > unmarshal/string_16_nsop.c
head -48 string_16.c | grep Unmarshal | sort -n -k 5 > unmarshal/string_16_Bop.c

head -48 string_3.c | grep Unmarshal | sort -n -k 3 > unmarshal/string_3_nsop.c
head -48 string_3.c | grep Unmarshal | sort -n -k 5 > unmarshal/string_3_Bop.c

head -48 embed_int32_\[0-5\].c | grep Unmarshal | sort -n -k 3 > unmarshal/embed_int32_\[0-5\]_nsop.c
head -48 embed_int32_\[0-5\].c | grep Unmarshal | sort -n -k 5 > unmarshal/embed_int32_\[0-5\]_Bop.c

head -48 embed_int32_\[0-32768\].c | grep Unmarshal | sort -n -k 3 > unmarshal/embed_int32_\[0-32768\]_nsop.c
head -48 embed_int32_\[0-32768\].c | grep Unmarshal | sort -n -k 5 > unmarshal/embed_int32_\[0-32768\]_Bop.c

head -48 embed_string_3.c | grep Unmarshal | sort -n -k 3 > unmarshal/embed_string_3_nsop.c
head -48 embed_string_3.c | grep Unmarshal | sort -n -k 5 > unmarshal/embed_string_3_Bop.c

