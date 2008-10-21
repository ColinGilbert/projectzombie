#extension GL_EXT_gpu_shader4 : enable
//This function initializes the 512bit data according to the MD5 spec. 
//Such that, the first 128 bit is the input;
//we also xor these 128 bits with the key, which can act like a seed value. 
//And the rest up of the 12 32bit data blocks are filled
//according to the md5 spec, in order to pad our data to 512 bits.
//block 0-3: input xor with key
//block 4: 0x80000000. This correponds to append 1 bit to block 0-4.
//block 5-13: 0. This corresponds to appending zeros up to 448 bit.
//block 14-15: 0x0000000000000080. This correspond to the bit length of the input (128 bit), as a 64bit 
//litten endian.
void setupInput(in uvec4 input, in unsigned int key, inout unsigned int data[16])
{
	data[0] = input.x^key; data[1] = input.y^key; data[2] = input.z^key; data[3] = input.w^key; //xor base with key
	data[4] = 0x80000000u;
	data[5] = 0u; data[6] = 0u; data[7] = 0u; data[8] = 0u; 
	data[9]=0u; data[10]=0u; data[11]=0u; data[12]=0u; data[13]=0u;
	data[14] = 0x00000000u; data[15]=0x00000080u; 
}
//initialize to the 4 hexes.
uvec4 initDigest()
{
	return uvec4(0x01234567u,0x89ABCDEFu,0xFEDCBA98u,0x76543210u);
}
//F compression functions
//(b & c) | ((not b) & d)
unsigned int F0_15(in uvec3 tD)
{
	return (tD.x & tD.y) | ((~tD.x) & tD.z);
}
//(d & b) | ((not d) & c)
unsigned int F16_31(in uvec3 tD)
{
	return (tD.z & tD.x) | ((~tD.z) & tD.y);
}
//b ^ c ^ d
unsigned int F32_47(in uvec3 tD)
{
	return tD.x ^ tD.y ^ tD.z;
}
//c ^ (b | (~d))
unsigned int F48_63(in uvec3 tD)
{
	return tD.y ^ (tD.x | (~tD.z));
}

const unsigned int RAND_MAX = 4294967295;

float conv(in unsigned int input)
{
	return float(input) / (float(RAND_MAX)+1.0);
}

vec4 convertToR0_R1(in uvec4 input)
{
	vec4 output;
	output.x = conv(input.x);
	output.y = conv(input.y);
	output.z = conv(input.z);
	output.w = conv(input.w);
	
	return output;
}

uvec4 whiteNoise(in uvec4 input,in unsigned int key)
{
	unsigned int data[16];
	setupInput(input,key,data);
	uvec4 rot0_15 = uvec4(7u,12u,17u,22u);
	uvec4 rot16_31 = uvec4(5u,9u,14u,20u);
	uvec4 rot32_47 = uvec4(4u,11u,16u,23u);
	uvec4 rot48_63 = uvec4(6u,10u,15u,21u);
	
	uvec4 digest = initDigest();
	uvec4 tD;
	uvec4 fTmp;
	unsigned int i = 0u;
	unsigned int idx;
	unsigned int r;
	unsigned int trig; const unsigned int MAXFT = 4294967295; //2^32-1
	//What follows is the unrolled loop from 0 through 63
	//0
	tD = digest;
	
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//1
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//2
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//3
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//4
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//5
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//6
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//7
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//8
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//9
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//10
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//11
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//12
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//13
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//14
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//15
	fTmp = F0_15(tD.yzw);
	idx = i++;
	r = rot0_15.x;
	rot0_15 = rot0_15.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	
	//16-31	
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//17
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//18
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//19
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//20
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//21
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//22
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//23
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//24
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//25
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//26
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//27
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//28
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//29
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//30
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//31
	fTmp = F16_31(tD.yzw);
	idx = (5u*i++ + 1u) % 16u;
	r = rot16_31.x;
	rot16_31 = rot16_31.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;

	//32-47
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//33
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//34
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//35
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//36
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//37
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//38
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//39
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//40
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//41
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//42
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//43
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//44
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//45
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//46
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//47
	fTmp = F32_47(tD.yzw);
	idx = (3u*i++ + 5u) % 16u;
	r = rot32_47.x;
	rot32_47 = rot32_47.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	
	//48-63
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//49
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//50
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//51
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//52
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//53
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//54
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//55
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//56
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//57
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//58
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//59
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//60
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//61
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//62
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	//63
	fTmp = F48_63(tD.yzw);
	idx = (7u*i++) % 16u;
	r = rot48_63.x;
	rot48_63 = rot48_63.yzwx;
	trig = truncate(abs(sin(float(i)))*float(MAXFT));
	tD.x = tD.y + ((tD.x+fTmp+data[int(idx)]+trig) << r);
	tD = tD.yzwx;
	digest +=tD;
	
	return digest; 	
} 