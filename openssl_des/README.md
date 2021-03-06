对称加密算法DES
概念：DES数据加解密就是将数据按照8个字节为单位进行DES加密或解密得到一段8个字节的密文或者明文，不足8个字节的以0x00或0xFF进行补齐；
应用场景：DES算法在POS、ATM、磁卡及智能卡（IC卡）、加油站、高速公路收费站等领域被广泛应用，以此来实现关键数据的保密，如信用卡持卡人的PIN的加密传输，IC卡与POS间的双向认证、金融交易数据包的MAC校验等，均用到DES算法。

ECB、CBC、CFB、OFB模式的区别
    ECB：Electronic Code Book，电子密码本模式，最基本的加密模式，也就是通常理解的加密，相同的明文将永远加密成相同的密文，无初始向量，容易受到密码本重放攻击，一般情况下很少用；关键在于各段分组数据是独立计算的，加密的最小单位仍然是8个字节；
	CBC：Cipher Block Chaining，密码分组链接，明文被加密前要与前面的密文进行异或运算后再加密，因此只要选择不同的初始向量，相同的密文加密后会形成不同的密文，这是目前应用最广泛的模式。CBC加密后的密文是上下文相关的，但明文的错误不会传递到后续分组，但如果一个分组丢失，后面的分组将全部作废(同步错误)。CBC则是对各段分组数据之间有所关联，可以理解为比ECB多了异或运算的步骤；
	CFB：Cipher FeedBack，密码反馈，类似于自同步序列密码，分组加密后，按8位分组将密文和明文进行移位异或后得到输出同时反馈回移位寄存器，优点最小可以按字节进行加解密，也可以是n位的，CFB也是上下文相关的，CFB模式下，明文的一个错误会影响后面的密文(错误扩散)。
	OFB：Output Feedback，输出反馈，将分组密码作为同步序列密码运行，和CFB相似，不过OFB用的是前一个n位密文输出分组反馈回移位寄存器，OFB没有错误扩散问题。

3DES算法
3DES算法顾名思义就是3次DES算法，其算法原理如下：设Ek()和Dk()代表DES算法的加密和解密过程，K代表DES算法使用的密钥，P代表明文，C代表密表；
3DES加密过程为：C=Ek3(Dk2(Ek1(P)))
3DES解密过程为：P=Dk1((EK2(Dk3(C)))
这里可以K1=K3，但不能K1=K2=K3（如果相等的话就成了DES算法了）；key长度支持24位、16位；3DES-CBC 流程基本与DES-CBC一样，多了两层加密解密来提供强度的支撑；

	3DES with 2 diffrent keys（K1=K3），可以是3DES-CBC，也可以是3DES-ECB，3DES-CBC整个算法的流程和DES-CBC一样，但是在原来的加密或者解密处增加了异或运算的步骤，使用的密钥是16字节长度的密钥，将密钥分成左8字节和右8字节的两部分，即k1=左8字节，k2=右8字节，然后进行加密运算和解密运算。

	3DES with 3 different keys，和3DES-CBC的流程完全一样，只是使用的密钥是24字节的，但在每个加密解密加密时候用的密钥不一样，将密钥分为3段8字节的密钥分别为密钥1、密钥2、密钥3，在3DES加密时对加密解密加密依次使用密钥1、密钥2、密钥3，在3DES解密时对解密加密解密依次使用密钥3、密钥2、密钥1。
