bin_list = []

for i in range(256):
	num_1s = (i>> 7 & 1) + (i>>6 & 1) + (i>>5 & 1) + (i>>4 & 1) + (i>>3 & 1) + (i>>2 & 1) + (i>>1 & 1) + (i & 1)
	bin_list.append(num_1s)

#print bin_list


huge_list_doubled = [
					[0x0,  0x0,  0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,  0x0,  0x0,   0x0],
					[0x0,  0x0,  0x0,  0x38,  0x38,  0x38,  0x38,  0x30,  0x30,  0x30,   0x0,  0x38,  0x38,  0x0,  0x0,   0x0], 
					[0x0,  0x0, 0x6c,  0x6c,  0x6c,  0x6c,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x36,  0x6e, 0x1fe,  0x6c,  0x6c,  0xec, 0x1fe,  0xdc,  0xd8,  0xd8,  0x0,  0x0,   0x0], 
					[0x0,  0x0, 0x30,  0x7c,  0xe4,  0xc0,  0xe0,  0xf8,  0x3c,   0xe,   0x6,  0xce,  0xfc, 0x30, 0x30,   0x0], 
					[0x0,  0x0,  0x0,  0xe6, 0x1e6, 0x1bc, 0x1f8,  0xf8,  0x3e,  0x7a,  0x7a,  0xda, 0x18e,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x78,  0xfc,  0xdc,  0xd8,  0x70,  0xf6, 0x1fe, 0x19c, 0x1de,  0xfe,  0x0,  0x0,   0x0], 
					[0x0,  0x0, 0x38,  0x38,  0x30,  0x30,  0x10,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,  0x0,  0x0,   0x0], 
					[0x0,  0xc, 0x1c,  0x38,  0x70,  0x60,  0x60,  0x60,  0x60,  0x60,  0x60,  0x70,  0x30, 0x38, 0x1c,   0x0], 
					[0x0,  0x0, 0x70,  0x30,  0x18,  0x1c,   0xc,   0xc,   0xc,   0xc,   0xc,  0x1c,  0x18, 0x38, 0x70,  0x60], 
					[0x0,  0x0,  0x0,  0x38,  0x30,  0xfe,  0x38,  0x7c,  0x6c,   0x0,   0x0,   0x0,   0x0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0x30,  0x30,  0x30, 0x1fe,  0x38,  0x30,  0x30,  0x10,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,  0x38,  0x38, 0x18, 0x38,  0x70], 
					[0x0,  0x0,  0x0,   0x0,   0x0,   0x0,   0x0,   0x0,  0x7c,   0x0,   0x0,   0x0,   0x0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,  0x30,  0x38,  0x38,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0xe,   0xc,   0xc,  0x1c,  0x18,  0x18,  0x38,  0x30,  0x30,  0x70,  0x60, 0x60, 0xe0,  0xc0], 
					[0x0,  0x0,  0x0,  0x7c,  0xee,  0xc6, 0x1fe, 0x1fe, 0x1c6, 0x1c6,  0xc6,  0xee,  0x7c,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x18,  0x78,  0xf8,  0x18,  0x18,  0x18,  0x18,  0x18,  0x18,  0xfe,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x7c,  0xfc,   0xe,   0xe,   0xc,  0x38,  0x70,  0x60,  0xe0,  0xfe,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xfc,  0xdc,   0xe,   0xc,  0x3c,   0xe,   0x6,   0x6,  0xce,  0xfc,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x1c,  0x3c,  0x3c,  0x6c,  0xcc,  0xcc, 0x1fe,   0xc,   0xc,   0xc,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x7e,  0x60,  0x60,  0x60,  0xfc,   0xe,   0x6,   0x6,  0xce,  0xfc,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x1e,  0x78,  0xe0,  0xfc,  0xce,  0xc6,  0xc6,  0xc6,  0xee,  0x7c,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xfe,   0xe,   0xc,  0x1c,  0x18,  0x38,  0x30,  0x30,  0x70,  0x70,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x7c,  0xee,  0xc6,  0xce,  0x7c,  0xfc,  0xc6, 0x1c6,  0xee,  0x7c,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x7c,  0xee,  0xc6, 0x1c6,  0xc6,  0xfe,   0xe,   0xc,  0x3c,  0xf0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0x38,  0x38,  0x30,   0x0,   0x0,  0x30,  0x38,  0x38,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0x38,  0x38,  0x30,   0x0,   0x0,   0x0,  0x38,  0x38, 0x38, 0x70,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,   0x2,  0x1e,  0x78, 0x1e0,  0xf0,  0x3e,   0x6,   0x0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,   0x0,   0x0, 0x1fe,   0x0,   0x0, 0x1fe,   0x0,   0x0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,   0x0,  0xf0,  0x3c,   0xe,  0x1e,  0xf8,  0xc0,   0x0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xfc,  0x4c,   0xe,   0xc,  0x18,  0x30,  0x20,   0x0,  0x70,  0x30,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x3c,  0x66,  0xc6,  0xde, 0x1fe, 0x1f6, 0x1f6, 0x1f6, 0x1fe,  0xde, 0xe0, 0x70,  0x3e], 
					[0x0,  0x0,  0x0,  0x38,  0x78,  0x7c,  0x6c,  0xec,  0xce,  0xfe, 0x1c6, 0x186, 0x186,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xfc,  0xce,  0xce,  0xcc,  0xfc,  0xce,  0xc6,  0xc6,  0xce,  0xfc,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x3e,  0xf6,  0xc0,  0xc0, 0x1c0, 0x1c0,  0xc0,  0xc0,  0xf6,  0x3e,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xf8,  0xdc,  0xce,  0xc6,  0xc6,  0xc6,  0xc6,  0xce,  0xdc,  0xf8,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xfe,  0xe0,  0xc0,  0xc0,  0xfc,  0xe0,  0xc0,  0xc0,  0xe0,  0xfe,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xfe,  0xe0,  0xc0,  0xc0,  0xfe,  0xe0,  0xc0,  0xc0,  0xc0,  0xc0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x3e,  0xf6,  0xc0,  0xc0, 0x1c0, 0x1c6,  0xc6,  0xc6,  0xf6,  0x3e,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0, 0x1c6, 0x1c6, 0x1c6, 0x1c6, 0x1fe, 0x1c6, 0x1c6, 0x1c6, 0x1c6, 0x1c6,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xfe,  0x38,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x38,  0xfe,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x7e,   0xe,   0xe,   0xe,   0xe,   0xe,   0xe,   0xe,  0xdc,  0xf8,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xc6,  0xcc,  0xdc,  0xf8,  0xf0,  0xf8,  0xdc,  0xcc,  0xce,  0xc6,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xc0,  0xc0,  0xc0,  0xc0,  0xc0,  0xc0,  0xc0,  0xc0,  0xe0,  0xfe,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xce,  0xee,  0xee,  0xfe, 0x1fe, 0x1be, 0x186, 0x186, 0x186, 0x186,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xc6,  0xe6,  0xe6,  0xf6,  0xf6,  0xde,  0xde,  0xce,  0xce,  0xc6,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x7c,  0xee, 0x1c6, 0x1c6, 0x186, 0x186, 0x1c6, 0x1c6,  0xee,  0x7c,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xfc,  0xce,  0xc6,  0xc6,  0xce,  0xfc,  0xc0,  0xc0,  0xc0,  0xc0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x7c,  0xee,  0xc6, 0x1c6, 0x186, 0x186, 0x186, 0x1c6,  0xc6,  0xfe, 0x7c, 0x38,  0x1e], 
					[0x0,  0x0,  0x0,  0xfc,  0xde,  0xce,  0xc6,  0xce,  0xfc,  0xd8,  0xcc,  0xce,  0xc6,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x7c,  0xe4,  0xc0,  0xe0,  0x78,  0x1c,   0x6,   0x6,  0xce,  0xfc,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0, 0x1fe,  0x38,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0, 0x1c6, 0x1c6, 0x1c6, 0x1c6, 0x1c6, 0x1c6, 0x1c6,  0xc6,  0xee,  0x7c,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0, 0x186, 0x1c6, 0x1c6,  0xc6,  0xce,  0xec,  0x6c,  0x7c,  0x78,  0x38,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0, 0x186, 0x186, 0x186, 0x186, 0x1be, 0x1fe,  0xfe,  0xee,  0xee,  0xce,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0, 0x1c6,  0xce,  0x6c,  0x78,  0x38,  0x78,  0x7c,  0xec,  0xc6, 0x1c6,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0, 0x186, 0x1c6,  0xce,  0xec,  0x7c,  0x38,  0x38,  0x38,  0x38,  0x38,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0xfe,   0xe,   0xc,  0x18,  0x38,  0x30,  0x60,  0xe0,  0xc0,  0xfe,  0x0,  0x0,   0x0], 
					[0x0,  0x0, 0x7c,  0x60,  0x60,  0x60,  0x60,  0x60,  0x60,  0x60,  0x60,  0x60,  0x60, 0x60, 0x60,  0x7c], 
					[0x0,  0x0, 0xc0,  0xe0,  0x60,  0x60,  0x70,  0x30,  0x30,  0x38,  0x18,  0x18,  0x1c,  0xc,  0xc,   0xe], 
					[0x0,  0x0, 0x7c,  0x1c,  0x1c,  0x1c,  0x1c,  0x1c,  0x1c,  0x1c,  0x1c,  0x1c,  0x1c, 0x1c, 0x1c,  0x7c], 
					[0x0,  0x0,  0x0,  0x38,  0x78,  0x7c,  0xec,  0xc6,  0x86,   0x0,   0x0,   0x0,   0x0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,  0x0,  0x0, 0x1fe], 
					[0x0,  0x0, 0x30,  0x18,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0x7c,   0xe,   0x6,  0x7e,  0xee,  0xc6,  0xee,  0x7e,  0x0,  0x0,   0x0], 
					[0x0, 0xc0, 0xc0,  0xc0,  0xc0,  0xfc,  0xee,  0xc6,  0xc6,  0xc6,  0xc6,  0xce,  0xfc,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0x7e,  0xf0,  0xc0, 0x1c0, 0x1c0,  0xc0,  0xf2,  0x7e,  0x0,  0x0,   0x0], 
					[0x0,  0x6,  0x6,   0x6,   0x6,  0x7e,  0xee, 0x1c6, 0x1c6, 0x1c6,  0xc6,  0xee,  0x7e,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0x7c,  0xee,  0xc6, 0x1fe, 0x1c0,  0xc0,  0xe4,  0x7e,  0x0,  0x0,   0x0], 
					[0x0,  0x0, 0x3e,  0x30,  0x70,  0xfe,  0x70,  0x70,  0x70,  0x70,  0x70,  0x70,  0x70,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0x7e,  0xee,  0xc6, 0x1c6, 0x1c6, 0x1c6,  0xee,  0x7e,  0xe, 0xcc,  0xfc], 
					[0x0, 0xc0, 0xc0,  0xc0,  0xc0,  0xfc,  0xee,  0xce,  0xc6,  0xc6,  0xc6,  0xc6,  0xc6,  0x0,  0x0,   0x0], 
					[0x0,  0x0, 0x70,  0x70,   0x0,  0xf0,  0x30,  0x30,  0x30,  0x30,  0x30,  0x3a,  0x1e,  0x0,  0x0,   0x0], 
					[0x0,  0x0, 0x1c,  0x1c,   0x0,  0x7c,   0xc,   0xc,   0xc,   0xc,   0xc,   0xc,   0xc,  0xc, 0xdc,  0xf8], 
					[0x0, 0xc0, 0xc0,  0xc0,  0xc0,  0xce,  0xdc,  0xf8,  0xf0,  0xf8,  0xdc,  0xce,  0xc6,  0x0,  0x0,   0x0], 
					[0x0,  0x0, 0xf0,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x3a,  0x1e,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0, 0x1fe, 0x1fe, 0x1b6, 0x1b6, 0x1b6, 0x186, 0x186, 0x186,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0xfc,  0xee,  0xce,  0xc6,  0xc6,  0xc6,  0xc6,  0xc6,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0x7c,  0xee,  0xc6, 0x1c6, 0x1c6,  0xc6,  0xee,  0x7c,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0xfc,  0xce,  0xc6,  0xc6,  0xc6,  0xc6,  0xee,  0xfc, 0xc0, 0xc0,  0xc0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0x7e,  0xee,  0xc6, 0x1c6, 0x1c6, 0x1c6,  0xee,  0x7e,  0x6,  0x6,   0x6], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0x7e,  0xe0,  0xe0,  0xe0,  0xe0,  0xe0,  0xe0,  0xe0,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0x7c,  0xe4,  0xc0,  0x78,  0x3c,   0xe,  0xce,  0xfc,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,  0x70,  0x70,  0xfe,  0x70,  0x70,  0x70,  0x70,  0x70,  0x3a,  0x3e,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0xc6,  0xc6,  0xc6,  0xc6,  0xc6,  0xc6,  0xee,  0x7e,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0, 0x1c6,  0xc6,  0xce,  0xec,  0x6c,  0x7c,  0x78,  0x38,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0, 0x182, 0x186, 0x1be, 0x1fe, 0x1fe,  0xee,  0xee,  0xce,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0xc6,  0xec,  0x7c,  0x38,  0x38,  0x6c,  0xce, 0x1c6,  0x0,  0x0,   0x0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0xc6,  0xc6,  0xee,  0x6c,  0x6c,  0x7c,  0x38,  0x38, 0x38, 0x70, 0x1e0], 
					[0x0,  0x0,  0x0,   0x0,   0x0,  0xfe,   0xc,  0x1c,  0x38,  0x70,  0x60,  0xe0,  0xfe,  0x0,  0x0,   0x0], 
					[0x0,  0x0, 0x1e,  0x30,  0x30,  0x30,  0x30,  0x30,  0xe0,  0x70,  0x30,  0x30,  0x30, 0x30, 0x30,  0x1e], 
					[0x0,  0x0, 0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30,  0x30, 0x30, 0x30,  0x30], 
					[0x0,  0x0, 0xf0,  0x38,  0x18,  0x18,  0x18,  0x18,  0x1e,  0x18,  0x18,  0x18,  0x18, 0x18, 0x38,  0xf0], 
					[0x0,  0x0,  0x0,   0x2,   0x2,   0x2,   0x2,   0x2,  0xf6, 0x1be,   0x2,   0x2,   0x2,  0x2,  0x2,   0x2]]

str_out = "{{"

value = 32

for i in range(len(huge_list_doubled)):
	for j in range(len(huge_list_doubled[0])):
		if(huge_list_doubled[i][j] / 2 >= 256):
			print("ERROR", huge_list_doubled[i][j])
		val = str(hex(huge_list_doubled[i][j] / 2))
		spaces = " " * (4 - len(val))
		str_out = str_out + spaces + val
		if(j +1 < len(huge_list_doubled[i])):
			str_out = str_out  + ", "
	if(i + 1 < len(huge_list_doubled)):
		str_out = str_out + "}, //" + chr(value)+ " ("+ str(value) +") \n{"
	value = value + 1
str_out = str_out + "}}; //" + chr(value-1) + " (" + str(value-1) + ")"
print str_out


def draw_character(ordinal):
	if(ordinal < 32 or ordinal >= 127):
		return
	bytes = huge_list_doubled[ordinal - 32]
	for byte in bytes:
		byte = byte >>1
		printed = ""
		for i in range(8):
			if(byte & 2**(7 - i)):
				printed = printed + "##"
			else:
				printed = printed + "  "
		print printed

while(True):
	inputed = raw_input("What character? ")
	try:
		value = int(inputed) + 32
	except ValueError:
		if(len(inputed) >0):
			value = ord(inputed[0])
			print(inputed[0], ord(inputed[0]))
		else:
			value = 32
	draw_character(value)