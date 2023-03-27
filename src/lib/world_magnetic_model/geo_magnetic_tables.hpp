/****************************************************************************
 *
 *   Copyright (c) 2020-2022 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include <stdint.h>

static constexpr float SAMPLING_RES = 10;
static constexpr float SAMPLING_MIN_LAT = -90;
static constexpr float SAMPLING_MAX_LAT = 90;
static constexpr float SAMPLING_MIN_LON = -180;
static constexpr float SAMPLING_MAX_LON = 180;

static constexpr int LAT_DIM = 19;
static constexpr int LON_DIM = 37;


// *INDENT-OFF*
// Magnetic declination data in radians * 10^-4
// Model: WMM-2020,
// Version: 0.5.1.11,
// Date: 2023.2329,
static constexpr const int16_t declination_table[19][37] {
	//    LONGITUDE:   -180,  -170,  -160,  -150,  -140,  -130,  -120,  -110,  -100,   -90,   -80,   -70,   -60,   -50,   -40,   -30,   -20,   -10,     0,    10,    20,    30,    40,    50,    60,    70,    80,    90,   100,   110,   120,   130,   140,   150,   160,   170,   180,
	/* LAT: -90 */ {  25970, 24224, 22479, 20734, 18988, 17243, 15498, 13752, 12007, 10262,  8516,  6771,  5026,  3281,  1535,  -210, -1955, -3701, -5446, -7191, -8936,-10682,-12427,-14172,-15918,-17663,-19408,-21154,-22899,-24645,-26390,-28135,-29881, 31206, 29460, 27715, 25970, },
	/* LAT: -80 */ {  22534, 20405, 18466, 16693, 15054, 13516, 12053, 10642,  9267,  7917,  6585,  5267,  3958,  2653,  1345,    26, -1316, -2689, -4100, -5553, -7049, -8587,-10165,-11785,-13451,-15170,-16956,-18830,-20813,-22933,-25208,-27643,-30211, 29983, 27366, 24863, 22534, },
	/* LAT: -70 */ {  14981, 13582, 12454, 11491, 10621,  9788,  8945,  8057,  7103,  6084,  5014,  3921,  2837,  1783,   762,  -248, -1286, -2393, -3597, -4900, -6283, -7715, -9165,-10608,-12035,-13452,-14882,-16369,-17996,-19915,-22445,-26256, 30657, 24120, 19621, 16852, 14981, },
	/* LAT: -60 */ {   8435,  8187,  7903,  7626,  7370,  7114,  6804,  6370,  5753,  4932,  3930,  2819,  1700,   675,  -202,  -957, -1685, -2510, -3519, -4722, -6058, -7431, -8754, -9965,-11029,-11927,-12642,-13136,-13308,-12859,-10752, -3451,  4969,  7693,  8453,  8574,  8435, },
	/* LAT: -50 */ {   5497,  5532,  5474,  5382,  5306,  5268,  5231,  5103,  4757,  4092,  3079,  1805,   466,  -709, -1572, -2129, -2522, -2963, -3657, -4682, -5935, -7218, -8360, -9260, -9855,-10089, -9888, -9125, -7612, -5246, -2339,   410,  2520,  3945,  4820,  5293,  5497, },
	/* LAT: -40 */ {   3963,  4055,  4061,  4015,  3952,  3917,  3921,  3909,  3734,  3196,  2169,   724,  -839, -2146, -3000, -3442, -3617, -3673, -3849, -4435, -5429, -6513, -7397, -7930, -8029, -7642, -6744, -5356, -3647, -1949,  -493,   728,  1773,  2641,  3299,  3731,  3963, },
	/* LAT: -30 */ {   2991,  3076,  3105,  3088,  3027,  2947,  2886,  2851,  2724,  2242,  1201,  -323, -1930, -3176, -3905, -4241, -4312, -4091, -3655, -3452, -3839, -4598, -5293, -5633, -5498, -4901, -3937, -2732, -1530,  -592,    81,   680,  1306,  1912,  2426,  2790,  2991, },
	/* LAT: -20 */ {   2348,  2393,  2409,  2408,  2363,  2266,  2157,  2080,  1934,  1435,   372, -1122, -2599, -3651, -4172, -4278, -4066, -3497, -2632, -1846, -1591, -1960, -2610, -3072, -3100, -2734, -2095, -1282,  -513,   -30,   228,   528,   971,  1455,  1881,  2193,  2348, },
	/* LAT: -10 */ {   1954,  1948,  1924,  1917,  1887,  1800,  1687,  1595,  1410,   857,  -211, -1592, -2861, -3680, -3934, -3695, -3113, -2328, -1483,  -734,  -279,  -324,  -800, -1304, -1511, -1416, -1101,  -604,  -112,   123,   166,   324,   698,  1141,  1541,  1832,  1954, },
	/* LAT:   0 */ {   1741,  1707,  1648,  1637,  1623,  1551,  1443,  1327,  1072,   450,  -593, -1821, -2870, -3446, -3432, -2923, -2152, -1372,  -723,  -189,   220,   324,    39,  -378,  -629,  -683,  -585,  -325,   -39,    45,   -22,    64,   409,   858,  1284,  1609,  1741, },
	/* LAT:  10 */ {   1603,  1611,  1566,  1580,  1604,  1552,  1426,  1237,   861,   143,  -879, -1953, -2773, -3103, -2889, -2277, -1500,  -791,  -279,   100,   423,   568,   401,    79,  -154,  -262,  -287,  -210,  -109,  -154,  -299,  -274,    31,   492,   978,  1390,  1603, },
	/* LAT:  20 */ {   1415,  1564,  1623,  1713,  1798,  1775,  1617,  1313,   768,   -87, -1128, -2075, -2665, -2767, -2433, -1828, -1117,  -470,   -12,   295,   553,   696,   602,   358,   160,    41,   -54,  -124,  -207,  -400,  -641,  -700,  -463,   -11,   535,  1056,  1415, },
	/* LAT:  30 */ {   1108,  1474,  1734,  1958,  2117,  2126,  1937,  1513,   786,  -241, -1354, -2225, -2634, -2566, -2164, -1584,  -928,  -312,   148,   452,   682,   826,   803,   654,   505,   379,   217,     0,  -287,  -665, -1035, -1194, -1032,  -602,   -24,   587,  1108, },
	/* LAT:  40 */ {    743,  1329,  1824,  2217,  2467,  2511,  2295,  1762,   853,  -370, -1602, -2461, -2777, -2622, -2175, -1582,  -929,  -300,   208,   571,   840,  1036,  1127,  1115,  1039,   889,   615,   196,  -348,  -957, -1478, -1720, -1601, -1183,  -584,    86,   743, },
	/* LAT:  50 */ {    445,  1191,  1873,  2432,  2802,  2910,  2683,  2032,   895,  -593, -2006, -2914, -3205, -3013, -2523, -1877, -1167,  -470,   145,   649,  1066,  1419,  1696,  1864,  1881,  1690,  1238,   520,  -381, -1284, -1960, -2242, -2115, -1674, -1042,  -315,   445, },
	/* LAT:  60 */ {    237,  1087,  1894,  2592,  3102,  3320,  3112,  2310,   809, -1133, -2837, -3813, -4066, -3808, -3238, -2494, -1669,  -830,   -23,   727,  1418,  2047,  2587,  2980,  3135,  2941,  2292,  1163,  -260, -1578, -2441, -2744, -2578, -2087, -1400,  -606,   237, },
	/* LAT:  70 */ {    -20,   912,  1809,  2610,  3229,  3531,  3294,  2188,     0, -2669, -4612, -5447, -5468, -4994, -4230, -3301, -2280, -1216,  -144,   915,  1939,  2903,  3769,  4474,  4916,  4940,  4317,  2823,   612, -1498, -2790, -3230, -3071, -2545, -1802,  -940,   -20, },
	/* LAT:  80 */ {   -799,   121,   971,  1663,  2052,  1892,   766, -1686, -4755, -6865, -7659, -7571, -6973, -6077, -5000, -3812, -2553, -1253,    68,  1393,  2705,  3987,  5214,  6354,  7351,  8113,  8456,  8001,  6023,  2135, -1527, -3235, -3584, -3248, -2568, -1719,  -799, },
	/* LAT:  90 */ { -29577,-27832,-26087,-24341,-22596,-20850,-19105,-17360,-15615,-13869,-12124,-10379, -8634, -6889, -5143, -3398, -1653,    92,  1837,  3582,  5328,  7073,  8818, 10563, 12309, 14054, 15799, 17545, 19290, 21036, 22781, 24527, 26272, 28018, 29763,-31323,-29577, },
};

// Magnetic inclination data in radians * 10^-4
// Model: WMM-2020,
// Version: 0.5.1.11,
// Date: 2023.2329,
static constexpr const int16_t inclination_table[19][37] {
	//    LONGITUDE:   -180,  -170,  -160,  -150,  -140,  -130,  -120,  -110,  -100,   -90,   -80,   -70,   -60,   -50,   -40,   -30,   -20,   -10,     0,    10,    20,    30,    40,    50,    60,    70,    80,    90,   100,   110,   120,   130,   140,   150,   160,   170,   180,
	/* LAT: -90 */ { -12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567,-12567, },
	/* LAT: -80 */ { -13651,-13517,-13356,-13176,-12982,-12781,-12577,-12377,-12186,-12010,-11854,-11720,-11610,-11523,-11460,-11418,-11398,-11401,-11428,-11482,-11566,-11680,-11825,-11999,-12198,-12415,-12644,-12877,-13105,-13317,-13504,-13653,-13757,-13808,-13804,-13749,-13651, },
	/* LAT: -70 */ { -14098,-13780,-13461,-13138,-12806,-12462,-12108,-11751,-11408,-11101,-10849,-10666,-10554,-10502,-10488,-10490,-10496,-10504,-10527,-10582,-10691,-10866,-11113,-11429,-11802,-12221,-12669,-13134,-13600,-14052,-14469,-14813,-14999,-14944,-14712,-14414,-14098, },
	/* LAT: -60 */ { -13514,-13160,-12822,-12489,-12146,-11773,-11359,-10904,-10437,-10008, -9680, -9506, -9505, -9643, -9846,-10037,-10159,-10198,-10183,-10167,-10215,-10375,-10667,-11078,-11578,-12136,-12722,-13319,-13907,-14469,-14966,-15255,-15075,-14689,-14282,-13888,-13514, },
	/* LAT: -50 */ { -12494,-12151,-11820,-11497,-11174,-10827,-10428, -9958, -9429, -8909, -8522, -8401, -8605, -9068, -9631,-10139,-10494,-10650,-10614,-10458,-10316,-10325,-10551,-10971,-11516,-12110,-12696,-13234,-13679,-13976,-14083,-14010,-13806,-13522,-13193,-12845,-12494, },
	/* LAT: -40 */ { -11239,-10890,-10541,-10196, -9856, -9518, -9158, -8734, -8214, -7650, -7228, -7194, -7661, -8488, -9409,-10234,-10887,-11315,-11445,-11269,-10923,-10655,-10655,-10942,-11408,-11913,-12357,-12676,-12833,-12838,-12748,-12612,-12434,-12202,-11915,-11587,-11239, },
	/* LAT: -30 */ {  -9602, -9221, -8840, -8449, -8057, -7682, -7327, -6938, -6426, -5816, -5369, -5464, -6245, -7453, -8703, -9793,-10702,-11404,-11792,-11771,-11391,-10883,-10556,-10563,-10819,-11140,-11394,-11503,-11439,-11263,-11091,-10959,-10815,-10610,-10329, -9982, -9602, },
	/* LAT: -20 */ {  -7372, -6928, -6507, -6078, -5634, -5205, -4816, -4406, -3842, -3158, -2718, -2995, -4123, -5743, -7369, -8730, -9786,-10552,-10979,-11002,-10629,-10011, -9456, -9218, -9271, -9434, -9573, -9587, -9413, -9141, -8951, -8869, -8770, -8565, -8248, -7835, -7372, },
	/* LAT: -10 */ {  -4417, -3875, -3417, -2978, -2520, -2071, -1659, -1210,  -588,   112,   464,    18, -1331, -3259, -5230, -6825, -7906, -8528, -8782, -8710, -8291, -7600, -6937, -6597, -6561, -6654, -6770, -6791, -6601, -6306, -6157, -6175, -6143, -5930, -5547, -5016, -4417, },
	/* LAT:   0 */ {   -910,  -279,   192,   597,  1015,  1432,  1819,  2256,  2834,  3406,  3598,  3085,  1771,  -150, -2191, -3825, -4816, -5231, -5280, -5102, -4652, -3929, -3224, -2858, -2799, -2867, -2990, -3062, -2932, -2698, -2650, -2804, -2878, -2697, -2272, -1638,  -910, },
	/* LAT:  10 */ {   2558,  3191,  3630,  3973,  4329,  4695,  5044,  5425,  5870,  6235,  6262,  5761,  4665,  3090,  1402,    40,  -749,  -983,  -883,  -642,  -224,   421,  1054,  1388,  1451,  1410,  1311,  1220,  1262,  1363,  1281,  1003,   800,   868,  1221,  1827,  2558, },
	/* LAT:  20 */ {   5414,  5947,  6329,  6626,  6940,  7283,  7623,  7966,  8294,  8489,  8395,  7925,  7081,  5981,  4857,  3957,  3439,  3331,  3487,  3734,  4069,  4539,  4999,  5253,  5311,  5297,  5251,  5193,  5180,  5158,  4979,  4639,  4334,  4241,  4413,  4839,  5414, },
	/* LAT:  30 */ {   7568,  7943,  8262,  8545,  8854,  9201,  9557,  9895, 10167, 10277, 10128,  9702,  9070,  8357,  7699,  7193,  6908,  6872,  7019,  7233,  7483,  7784,  8073,  8247,  8305,  8321,  8325,  8315,  8291,  8206,  7980,  7623,  7263,  7041,  7026,  7222,  7568, },
	/* LAT:  40 */ {   9266,  9487,  9743, 10029, 10355, 10715, 11081, 11418, 11668, 11747, 11594, 11233, 10759, 10287,  9893,  9613,  9467,  9467,  9581,  9744,  9922, 10107, 10280, 10405, 10483, 10546, 10602, 10636, 10617, 10505, 10260,  9907,  9538,  9253,  9110,  9122,  9266, },
	/* LAT:  50 */ {  10802, 10923, 11124, 11393, 11715, 12068, 12421, 12736, 12956, 13014, 12873, 12576, 12212, 11868, 11594, 11410, 11319, 11318, 11387, 11491, 11607, 11723, 11840, 11956, 12075, 12199, 12313, 12385, 12375, 12248, 12000, 11671, 11330, 11045, 10856, 10776, 10802, },
	/* LAT:  60 */ {  12319, 12391, 12540, 12757, 13026, 13325, 13627, 13892, 14067, 14095, 13959, 13711, 13423, 13153, 12934, 12779, 12689, 12660, 12677, 12725, 12794, 12880, 12987, 13120, 13280, 13456, 13621, 13729, 13733, 13611, 13385, 13106, 12828, 12591, 12421, 12329, 12319, },
	/* LAT:  70 */ {  13758, 13799, 13892, 14033, 14211, 14413, 14621, 14801, 14906, 14888, 14753, 14554, 14337, 14134, 13961, 13827, 13733, 13679, 13661, 13674, 13717, 13790, 13894, 14031, 14198, 14386, 14570, 14710, 14754, 14680, 14518, 14320, 14126, 13961, 13840, 13771, 13758, },
	/* LAT:  80 */ {  14994, 15006, 15042, 15100, 15176, 15259, 15337, 15383, 15368, 15292, 15182, 15060, 14938, 14824, 14725, 14643, 14582, 14541, 14523, 14527, 14554, 14603, 14675, 14768, 14880, 15007, 15143, 15275, 15383, 15426, 15385, 15297, 15201, 15116, 15050, 15009, 14994, },
	/* LAT:  90 */ {  15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15397, 15397, 15397, 15397, 15397, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, 15398, },
};

// Magnetic strength data in milli-Gauss * 10
// Model: WMM-2020,
// Version: 0.5.1.11,
// Date: 2023.2329,
static constexpr const int16_t strength_table[19][37] {
	//    LONGITUDE:  -180, -170, -160, -150, -140, -130, -120, -110, -100,  -90,  -80,  -70,  -60,  -50,  -40,  -30,  -20,  -10,    0,   10,   20,   30,   40,   50,   60,   70,   80,   90,  100,  110,  120,  130,  140,  150,  160,  170,  180,
	/* LAT: -90 */ {  5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, 5447, },
	/* LAT: -80 */ {  6054, 5990, 5911, 5818, 5715, 5603, 5485, 5363, 5241, 5121, 5007, 4902, 4808, 4729, 4666, 4621, 4597, 4595, 4617, 4663, 4734, 4828, 4943, 5075, 5219, 5369, 5519, 5662, 5793, 5907, 5999, 6068, 6112, 6131, 6127, 6100, 6054, },
	/* LAT: -70 */ {  6298, 6165, 6014, 5848, 5669, 5477, 5271, 5057, 4838, 4623, 4420, 4238, 4081, 3950, 3846, 3770, 3723, 3711, 3740, 3816, 3945, 4126, 4355, 4623, 4919, 5226, 5530, 5814, 6064, 6267, 6418, 6513, 6554, 6546, 6496, 6411, 6298, },
	/* LAT: -60 */ {  6184, 5991, 5789, 5579, 5360, 5125, 4868, 4590, 4298, 4008, 3742, 3515, 3336, 3202, 3103, 3030, 2979, 2959, 2986, 3077, 3247, 3499, 3827, 4214, 4637, 5071, 5493, 5876, 6201, 6449, 6612, 6690, 6691, 6627, 6513, 6361, 6184, },
	/* LAT: -50 */ {  5842, 5611, 5378, 5147, 4914, 4669, 4398, 4095, 3766, 3434, 3133, 2894, 2733, 2642, 2594, 2559, 2525, 2498, 2503, 2577, 2752, 3044, 3442, 3915, 4422, 4928, 5403, 5821, 6159, 6400, 6538, 6578, 6535, 6424, 6261, 6062, 5842, },
	/* LAT: -40 */ {  5392, 5145, 4899, 4659, 4424, 4187, 3932, 3648, 3334, 3008, 2710, 2488, 2372, 2347, 2366, 2386, 2390, 2377, 2364, 2393, 2526, 2804, 3225, 3741, 4288, 4811, 5277, 5664, 5956, 6143, 6231, 6233, 6163, 6031, 5850, 5631, 5392, },
	/* LAT: -30 */ {  4877, 4636, 4396, 4161, 3935, 3716, 3497, 3265, 3009, 2734, 2476, 2295, 2226, 2252, 2318, 2389, 2454, 2503, 2523, 2535, 2604, 2807, 3172, 3663, 4195, 4691, 5108, 5423, 5625, 5725, 5751, 5723, 5644, 5511, 5331, 5114, 4877, },
	/* LAT: -20 */ {  4320, 4107, 3898, 3692, 3496, 3314, 3145, 2981, 2803, 2605, 2414, 2281, 2241, 2286, 2376, 2486, 2614, 2741, 2827, 2862, 2888, 2987, 3230, 3617, 4068, 4494, 4841, 5074, 5178, 5185, 5155, 5108, 5026, 4898, 4731, 4534, 4320, },
	/* LAT: -10 */ {  3790, 3628, 3475, 3328, 3192, 3072, 2969, 2876, 2778, 2663, 2541, 2442, 2398, 2424, 2512, 2641, 2797, 2954, 3076, 3137, 3151, 3180, 3306, 3560, 3887, 4209, 4473, 4634, 4668, 4615, 4548, 4484, 4396, 4271, 4122, 3958, 3790, },
	/* LAT:   0 */ {  3412, 3318, 3234, 3161, 3105, 3067, 3041, 3021, 2996, 2947, 2868, 2774, 2695, 2666, 2709, 2813, 2945, 3080, 3194, 3268, 3299, 3321, 3398, 3557, 3767, 3981, 4160, 4265, 4270, 4202, 4114, 4022, 3910, 3779, 3645, 3520, 3412, },
	/* LAT:  10 */ {  3282, 3251, 3230, 3226, 3250, 3297, 3352, 3405, 3439, 3429, 3359, 3245, 3119, 3026, 3001, 3044, 3125, 3224, 3324, 3408, 3472, 3535, 3624, 3743, 3879, 4018, 4137, 4207, 4209, 4146, 4036, 3893, 3731, 3572, 3436, 3339, 3282, },
	/* LAT:  20 */ {  3399, 3401, 3427, 3480, 3572, 3692, 3820, 3937, 4017, 4029, 3955, 3814, 3648, 3509, 3435, 3424, 3460, 3533, 3630, 3728, 3818, 3917, 4029, 4141, 4250, 4363, 4465, 4531, 4540, 4481, 4343, 4141, 3913, 3703, 3540, 3439, 3399, },
	/* LAT:  30 */ {  3722, 3728, 3782, 3880, 4022, 4193, 4368, 4523, 4631, 4658, 4585, 4430, 4242, 4079, 3975, 3930, 3935, 3986, 4074, 4173, 4271, 4377, 4493, 4611, 4731, 4859, 4980, 5066, 5091, 5033, 4876, 4636, 4361, 4107, 3907, 3778, 3722, },
	/* LAT:  40 */ {  4222, 4219, 4282, 4405, 4571, 4758, 4940, 5097, 5201, 5227, 5159, 5012, 4826, 4653, 4526, 4452, 4427, 4450, 4511, 4591, 4678, 4776, 4891, 5024, 5175, 5338, 5489, 5599, 5637, 5582, 5425, 5187, 4913, 4654, 4443, 4297, 4222, },
	/* LAT:  50 */ {  4832, 4823, 4877, 4985, 5131, 5290, 5439, 5561, 5635, 5644, 5582, 5455, 5294, 5131, 4995, 4898, 4844, 4833, 4858, 4909, 4979, 5070, 5190, 5340, 5515, 5700, 5866, 5983, 6026, 5981, 5850, 5654, 5430, 5215, 5035, 4904, 4832, },
	/* LAT:  60 */ {  5392, 5378, 5404, 5466, 5551, 5644, 5732, 5799, 5833, 5825, 5771, 5678, 5558, 5430, 5312, 5217, 5151, 5117, 5116, 5144, 5201, 5288, 5406, 5553, 5720, 5888, 6035, 6138, 6181, 6158, 6075, 5949, 5804, 5662, 5539, 5447, 5392, },
	/* LAT:  70 */ {  5726, 5705, 5701, 5711, 5733, 5759, 5783, 5798, 5798, 5780, 5742, 5685, 5616, 5540, 5467, 5403, 5355, 5328, 5324, 5344, 5389, 5458, 5550, 5659, 5776, 5890, 5989, 6061, 6100, 6103, 6074, 6021, 5954, 5884, 5818, 5764, 5726, },
	/* LAT:  80 */ {  5790, 5772, 5757, 5744, 5734, 5725, 5715, 5703, 5689, 5671, 5649, 5624, 5598, 5571, 5547, 5526, 5513, 5508, 5513, 5529, 5554, 5590, 5632, 5680, 5730, 5778, 5820, 5855, 5879, 5892, 5894, 5887, 5872, 5853, 5832, 5810, 5790, },
	/* LAT:  90 */ {  5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, 5683, },
};
