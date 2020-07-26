// full adder with 0 carry in
module FA_0( CO,S,A,B,n );
	output S,CO;
	input A,B;
	
	output wire[50:0] n;
	wire [50:0] n1, n2;
	assign n = n1 + n2;
	
	EO _eo(S, A, B, n1);
	AN2 _an2(CO, A, B, n2);
endmodule

// full adder with 1 carry in
module FA_1( CO,S,A,B,n );
	output S,CO;
	input A,B;
	
	output wire[50:0] n;
	wire [50:0] n1, n2, n3;
	wire InvS;
	assign n = n1 + n2 + n3;
	
	EO _eo(InvS, A, B, n1);
	IV _iv(S, InvS, n3);
	OR2 _or2(CO, A, B, n2);
endmodule

// 10bit DFF
module DFF10( out, in, _clk, _rst, n );
	output [9:0] out;
	input [9:0] in;
	input _clk, _rst;
	output wire [50:0] n;
	wire [50:0] n0,n1,n2,n3,n4,n5,n6,n7,n8,n9;
	assign n = n0+n1+n2+n3+n4+n5+n6+n7+n8+n9;
	
	FD2 _pos0( out[0] , in[0] , _clk , _rst , n0 );
	FD2 _pos1( out[1] , in[1] , _clk , _rst , n1 );
	FD2 _pos2( out[2] , in[2] , _clk , _rst , n2 );
	FD2 _pos3( out[3] , in[3] , _clk , _rst , n3 );
	FD2 _pos4( out[4] , in[4] , _clk , _rst , n4 );
	FD2 _pos5( out[5] , in[5] , _clk , _rst , n5 );
	FD2 _pos6( out[6] , in[6] , _clk , _rst , n6 );
	FD2 _pos7( out[7] , in[7] , _clk , _rst , n7 );
	FD2 _pos8( out[8] , in[8] , _clk , _rst , n8 );
	FD2 _pos9( out[9] , in[9] , _clk , _rst , n9 );
	
endmodule

// 23bit DFF
module DFF23( out, in, _clk, _rst, n );
	output [22:0] out;
	input [22:0] in;
	input _clk, _rst;
	output wire [50:0] n;
	wire [50:0] n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22;
	assign n = n0+n1+n2+n3+n4+n5+n6+n7+n8+n9+n10+n11+n12+n13+n14+n15+n16+n17+n18+n19+n20+n21+n22;
	
	FD2 _pos0( out[0] , in[0] , _clk , _rst , n0 );
	FD2 _pos1( out[1] , in[1] , _clk , _rst , n1 );
	FD2 _pos2( out[2] , in[2] , _clk , _rst , n2 );
	FD2 _pos3( out[3] , in[3] , _clk , _rst , n3 );
	FD2 _pos4( out[4] , in[4] , _clk , _rst , n4 );
	FD2 _pos5( out[5] , in[5] , _clk , _rst , n5 );
	FD2 _pos6( out[6] , in[6] , _clk , _rst , n6 );
	FD2 _pos7( out[7] , in[7] , _clk , _rst , n7 );
	FD2 _pos8( out[8] , in[8] , _clk , _rst , n8 );
	FD2 _pos9( out[9] , in[9] , _clk , _rst , n9 );
	FD2 _pos10( out[10] , in[10] , _clk , _rst , n10 );
	FD2 _pos11( out[11] , in[11] , _clk , _rst , n11 );
	FD2 _pos12( out[12] , in[12] , _clk , _rst , n12 );
	FD2 _pos13( out[13] , in[13] , _clk , _rst , n13 );
	FD2 _pos14( out[14] , in[14] , _clk , _rst , n14 );
	FD2 _pos15( out[15] , in[15] , _clk , _rst , n15 );
	FD2 _pos16( out[16] , in[16] , _clk , _rst , n16 );
	FD2 _pos17( out[17] , in[17] , _clk , _rst , n17 );
	FD2 _pos18( out[18] , in[18] , _clk , _rst , n18 );
	FD2 _pos19( out[19] , in[19] , _clk , _rst , n19 );
	FD2 _pos20( out[20] , in[20] , _clk , _rst , n20 );
	FD2 _pos21( out[21] , in[21] , _clk , _rst , n21 );
	FD2 _pos22( out[22] , in[22] , _clk , _rst , n22 );
	
endmodule

// 21bit DFF
module DFF21( out, in, _clk, _rst, n );
	output [20:0] out;
	input [20:0] in;
	input _clk, _rst;
	output wire [50:0] n;
	wire [50:0] n0,n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20;
	assign n = n0+n1+n2+n3+n4+n5+n6+n7+n8+n9+n10+n11+n12+n13+n14+n15+n16+n17+n18+n19+n20;
	
	FD2 _pos0( out[0] , in[0] , _clk , _rst , n0 );
	FD2 _pos1( out[1] , in[1] , _clk , _rst , n1 );
	FD2 _pos2( out[2] , in[2] , _clk , _rst , n2 );
	FD2 _pos3( out[3] , in[3] , _clk , _rst , n3 );
	FD2 _pos4( out[4] , in[4] , _clk , _rst , n4 );
	FD2 _pos5( out[5] , in[5] , _clk , _rst , n5 );
	FD2 _pos6( out[6] , in[6] , _clk , _rst , n6 );
	FD2 _pos7( out[7] , in[7] , _clk , _rst , n7 );
	FD2 _pos8( out[8] , in[8] , _clk , _rst , n8 );
	FD2 _pos9( out[9] , in[9] , _clk , _rst , n9 );
	FD2 _pos10( out[10] , in[10] , _clk , _rst , n10 );
	FD2 _pos11( out[11] , in[11] , _clk , _rst , n11 );
	FD2 _pos12( out[12] , in[12] , _clk , _rst , n12 );
	FD2 _pos13( out[13] , in[13] , _clk , _rst , n13 );
	FD2 _pos14( out[14] , in[14] , _clk , _rst , n14 );
	FD2 _pos15( out[15] , in[15] , _clk , _rst , n15 );
	FD2 _pos16( out[16] , in[16] , _clk , _rst , n16 );
	FD2 _pos17( out[17] , in[17] , _clk , _rst , n17 );
	FD2 _pos18( out[18] , in[18] , _clk , _rst , n18 );
	FD2 _pos19( out[19] , in[19] , _clk , _rst , n19 );
	FD2 _pos20( out[20] , in[20] , _clk , _rst , n20 );
	
endmodule

module lin_class (
	input i_clk, input i_rst_n, input [5:0] i_im1, input [5:0] i_im2, input [5:0] i_im3,
	output [15:0] o_wgt_sum, output o_pos, output [50:0] number );
	
	wire [9:0] s0, s1, s2, s3, s4, s5, s6, s7, s9, s10, s11, s12, s3_i; //s8 is s2
	
	/* 5x + y */
	wire [50:0] n0, n00, n01, n02, n03, n04, n05, n06, n07;
	
	HA1 ha5xy_1( s1[1], s0[0], i_im1[0], i_im2[0], n00 );
	HA1 ha5xy_2( s1[2], s0[1], i_im1[1], i_im2[1], n01 );
	FA1 fa5xy_1( s1[3], s0[2], i_im1[2], i_im2[2], i_im1[0], n02 );
	FA1 fa5xy_2( s1[4], s0[3], i_im1[1], i_im2[3], i_im1[3], n03 );
	FA1 fa5xy_3( s1[5], s0[4], i_im1[2], i_im2[4], i_im1[4], n04 );
	FA_1 fa5xy_4( s1[6], s0[5], i_im2[5], i_im1[3], n05 );
	assign s1[7] = i_im1[4];
	IV iv5xy_1(s0[6], i_im1[4], n06 );
	IV iv5xy_2(s0[7], i_im1[5], n07 );
	assign s2[5] = s0[7];
	assign n0 = n00 + n01 + n02 + n03 + n04 + n05 + n06 + n07;
	
	/* 9z + 74 */
	wire [50:0] n1, n10, n11, n12, n13, n14, n15, n16, n17, n18, n19;
	
	IV iv9z74_0(s3[0], i_im3[0], n10 ); // switched
	assign s4[2] = i_im3[1];
	IV iv9z74_1(s3[1], i_im3[1], n11 );
	IV iv9z74_2(s3[2], i_im3[2], n12 );
	FA_1 fa9z74_1(s4[4], s3[3], i_im3[0], i_im3[3], n13 );
	HA1 ha9z74_1( s4[5], s3[4], i_im3[4], i_im3[1], n14 );
	IV iv9z74_3(s2[0], i_im3[5], n15 ); // temp s2[0]
	FA_1 fa9z74_2( s4[6], s3[5], i_im3[2], s2[0], n16 );
	IV iv9z74_4(s3[6], i_im3[3], n17 ); // switched
	IV iv9z74_5(s3[7], i_im3[4], n18 ); // switched
	assign s3[8] = i_im3[5];
	IV iv9z74_6(s4[9], i_im3[5], n19 ); // delete later
	assign n1 = n10 + n11 + n12 + n13 + n14 + n15 + n16 + n17 + n18 + n19;
	
	/* 5x + y  + ~(9z + 74) */
	wire [50:0] n2, n20, n21, n22, n23, n24, n25, n26, n27, n28, n29;
	wire [50:0] n210, n211, n212, n213, n214, n215, n216, n217;
	
	FA_1 faALL_1( s6[1], s5[0], s0[0], s3[0], n20 );
	IV ivALL_0(s3_i[1], s3[1], n21 ); // temp
	FA1 faALL_2( s6[2], s5[1], s0[1], s1[1], s3_i[1], n22 );
	FA1 faALL_3( s6[3], s5[2], s0[2], s1[2], s3[2], n23 );
	IV ivALL_1(s3_i[3], s3[3], n24 ); // temp
	FA1 faALL_4( s6[4], s5[3], s0[3], s1[3], s3_i[3], n25 );
	IV ivALL_2(s3_i[4], s3[4], n26 ); // temp
	FA1 faALL_5( s6[5], s5[4], s0[4], s1[4], s3_i[4], n27 );
	FA1 faALL_6( s6[6], s5[5], s0[5], s1[5], s2[5], n28 );
	FA1 faALL_7( s6[7], s5[6], s0[6], s1[6], s3[6], n29 );
	FA1 faALL_8( s6[8], s5[7], s0[7], s1[7], s3[7], n210 );
	IV ivALL_3(s2[1], s4[5], n211 ); // temp s2[1]
	IV ivALL_4(s2[2], s3[5], n212 ); // temp s2[2]
	HA1 haALL_1( s7[6], s7[5], s2[1], s2[2], n213 );
	assign s5[9] = i_im3[5];
	IV ivALL_5(s5[8], s3[8], n214 );
	assign s7[7] = 1'b1;
	assign s7[3] = 1'b1;
	assign s7[1] = 1'b1;
	IV ivALL_6(s7[4], s4[4], n215 );
	IV ivALL_7(s7[2], s4[2], n216 );
	IV ivALL_8(s2[6], s4[6], n217 ); //s8 is s2
	assign n2 = n20 + n21 + n22 + n23 + n24 + n25 + n26 + n27 + n28 + n29 + n210 + n211 + n212 + n213 + n214 + n215 + n216 + n217;
	
	// flip flops
	wire [50:0] n3;
	
	wire [22:0] i0, i1, i2, i3;
	DFF23 _dff0( i0, {s5[9:0],s6[8:1],s7[6:4],s7[2],s2[6]}, i_clk, i_rst_n, n3 );
	
	//wire [17:0] i0, i1;
	//wire [17:0] wgt;
	//assign wgt = i_im1*5 + i_im2 - i_im3*9 - 76;
	
	/* Squeeze 1 */
	wire [50:0] n4, n40, n41, n42, n43, n44, n45, n46, n47;
	
	assign s9[0] = i0[13];
	assign s9[9] = i0[22];
	//IV ivSQZ_2(s11[6], i0[0]); i0[0] = s11[6] = !s4[6] prev
	FA_1 faSQZ_1( s10[2], s9[1], i0[5], i0[14], n40 );
	FA1 faSQZ_2( s10[3], s9[2], i0[6], i0[15], i0[1], n41 );
	FA_1 faSQZ_3( s10[4], s9[3], i0[7], i0[16], n42 );
	FA1 faSQZ_4( s10[5], s9[4], i0[8], i0[17], i0[2], n43 );
	FA1 faSQZ_5( s10[6], s9[5], i0[9], i0[18], i0[3], n44 );
	FA1 faSQZ_6( s10[7], s9[6], i0[10], i0[19], i0[4], n45 );
	FA_1 faSQZ_7( s10[8], s9[7], i0[11], i0[20], n46 );
	HA1 haSQZ_1( s10[9], s9[8], i0[12], i0[21], n47 );
	assign n4 = n40 + n41 + n42 + n43 + n44 + n45 + n46 + n47;
	
	/* Final squeeze */
	wire [50:0] n5, n50, n51, n52, n53;
	
	FA1 faFSQZ_1( s12[7], s11[6], i0[0], s9[6], s10[6], n50 );
	HA1 haFSQZ_1( s12[8], s11[7], s9[7], s10[7], n51 );
	HA1 haFSQZ_2( s12[9], s11[8], s9[8], s10[8], n52 );
	EO eoFSQZ_1( s11[9], s9[9], s10[9], n53 );
	assign n5 = n50 + n51 + n52 + n53;
	
	/* Carry-Select lower 4 bit (5:2) */
	wire [50:0] n6, n60, n61, n62, n63, n64, n65;
	assign i1[0] = s9[0];
	assign i1[1] = s9[1];
	HA1 haCSL4_1( s12[2], i1[2], s9[2], s10[2], n60  ); // s12[5:2] as carry of s9 s10
	FA1 faCSL4_1( s12[3], i1[3], s9[3], s10[3], s12[2], n61 );
	wire i14_0, i14_1, c5_0, i15_0, i15_1, c5_1; // selector
	wire ANDs94s104; //if c3 = 0
	HA1 haCSL4_2( ANDs94s104, i14_0, s9[4], s10[4], n62 );
	FA1 faCSL4_2( c5_0, i15_0, s9[5], s10[5], ANDs94s104, n63 );
	wire ORs94s104; //if c3 = 1
	FA_1 faCSL4_3( ORs94s104, i14_1, s9[4], s10[4], n64 );
	FA1 faCSL4_4( c5_1, i15_1, s9[5], s10[5], ORs94s104, n65 );
	
	assign n6 = n60 + n61 + n62 + n63 + n64 + n65;
	
	/* Carry-Select upper 4 bit (9:6) */
	wire [50:0] n7, n70, n71, n72, n73;
	
	wire i16_0, i16_1, i17_0, i17_1, i18_0, i18_1, i19_0, i19_1, c7_1, c8_0, c8_1;
	// if c[5] = 0
	assign i16_0 = s11[6];
	EO eoCSU4_0( i17_0, s11[7], s12[7], n70 );
	wire ANDs117s127, XORs119s129;
	AN2 anCSU4_1( ANDc117c127, s11[7], s12[7], n71 );
	// if c[5] = 1
	IV ivCSU4_1( i16_1, s11[6], n72 );
	FA1 faCSU4_2( c7_1, i17_1, s11[7], s12[7], s11[6], n73 );
	
	assign n7 = n70 + n71 + n72 + n73;
	
	wire [50:0] n8, n9, n_10;
	DFF21 _d1( i2[20:0], { i1[2], i1[3], ANDc117c127, c7_1, i16_0, i16_1, i17_0, i17_1, s11[8], s11[9], s12[8], s12[9], i14_0, i14_1, i15_0, i15_1, c5_0, c5_1, s12[3], i1[1:0] }, i_clk, i_rst_n, n8 );
	
	assign i3[0] = i2[0];
	assign i3[1] = i2[1];
	assign i3[2] = i2[20];
	assign i3[3] = i2[19];
	
	/* Carry-Select lower 4 bit (5:2) */
	wire [50:0] n_11, n_110, n_111, n_112, n_113;
	
	wire ANDc3c5_1; // last connection
	AN2 anCSL4_1( ANDc3c5_1, i2[2], i2[3], n_110 );
	OR2 orCSL4_1( s12[5], ANDc3c5_1, i2[4], n_111 );
	MUX21H muxCSL4_1( i3[4], i2[8], i2[7], i2[2], n_112 );
	MUX21H muxCSL4_2( i3[5], i2[6], i2[5], i2[2], n_113 );
	
	assign n_11 = n_110 + n_111 + n_112 + n_113;
	
	/* Carry-Select upper 4 bit (9:6) */
	// if c[5] = 0
	wire [50:0] n_12, n_120, n_121, n_122, n_123, n_124, n_125, n_126, n_127, n_128;
	
	FA1 faCSU4_1( c8_0, i18_0, i2[12], i2[10], i2[18], n_120 );
	EO eoCSU4_1( XORs119s129, i2[11], i2[9], n_121 );
	EO eoCSU4_2( i19_0, XORs119s129, c8_0, n_122 );
	// if c[5] = 1
	FA1 faCSU4_3( c8_1, i18_1, i2[12], i2[10], i2[17], n_123 );
	EO eoCSU4_3( i19_1, XORs119s129, c8_1, n_124 );
	// MUX41 (9:6)
	MUX21H muxCSU4_1( i3[6], i2[16], i2[15], s12[5], n_125 );
	MUX21H muxCSU4_2( i3[7], i2[14], i2[13], s12[5], n_126 );
	MUX21H muxCSU4_3( i3[8], i18_0, i18_1, s12[5], n_127 );
	MUX21H muxCSU4_4( i3[9], i19_0, i19_1, s12[5], n_128 );
	
	assign n_12 = n_120 + n_121 + n_122 + n_123 + n_124 + n_125 + n_126 + n_127 + n_128;
	
	DFF10 _d2( o_wgt_sum[9:0], i3[9:0], i_clk, i_rst_n, n9 );
	
	assign o_wgt_sum[10] = o_wgt_sum[9];
	assign o_wgt_sum[11] = o_wgt_sum[9];
	assign o_wgt_sum[12] = o_wgt_sum[9];
	assign o_wgt_sum[13] = o_wgt_sum[9];
	assign o_wgt_sum[14] = o_wgt_sum[9];
	assign o_wgt_sum[15] = o_wgt_sum[9];
	
	IV _iv( o_pos, o_wgt_sum[15], n_10 );
	
	assign number = n0 + n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 + n9 + n_10 + n_11 + n_12;
	
endmodule
