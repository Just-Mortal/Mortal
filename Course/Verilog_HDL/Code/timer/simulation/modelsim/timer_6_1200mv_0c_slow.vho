-- Copyright (C) 1991-2013 Altera Corporation
-- Your use of Altera Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Altera Program License 
-- Subscription Agreement, Altera MegaCore Function License 
-- Agreement, or other applicable license agreement, including, 
-- without limitation, that your use is for the sole purpose of 
-- programming logic devices manufactured by Altera and sold by 
-- Altera or its authorized distributors.  Please refer to the 
-- applicable agreement for further details.

-- VENDOR "Altera"
-- PROGRAM "Quartus II 64-Bit"
-- VERSION "Version 13.1.0 Build 162 10/23/2013 SJ Full Version"

-- DATE "12/24/2024 14:52:27"

-- 
-- Device: Altera EP4CGX15BF14C6 Package FBGA169
-- 

-- 
-- This VHDL file should be used for ModelSim-Altera (VHDL) only
-- 

LIBRARY ALTERA;
LIBRARY CYCLONEIV;
LIBRARY IEEE;
USE ALTERA.ALTERA_PRIMITIVES_COMPONENTS.ALL;
USE CYCLONEIV.CYCLONEIV_COMPONENTS.ALL;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY 	timer IS
    PORT (
	reset : IN std_logic;
	clk : IN std_logic;
	out_sec_l : OUT std_logic_vector(6 DOWNTO 0);
	out_sec_h : OUT std_logic_vector(6 DOWNTO 0);
	out_min_l : OUT std_logic_vector(6 DOWNTO 0);
	out_min_h : OUT std_logic_vector(6 DOWNTO 0);
	out_hour : OUT std_logic_vector(6 DOWNTO 0)
	);
END timer;

-- Design Ports Information
-- out_sec_l[0]	=>  Location: PIN_N6,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_l[1]	=>  Location: PIN_A12,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_l[2]	=>  Location: PIN_M4,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_l[3]	=>  Location: PIN_L13,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_l[4]	=>  Location: PIN_A7,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_l[5]	=>  Location: PIN_A8,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_l[6]	=>  Location: PIN_A6,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_h[0]	=>  Location: PIN_L4,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_h[1]	=>  Location: PIN_M6,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_h[2]	=>  Location: PIN_N8,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_h[3]	=>  Location: PIN_B6,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_h[4]	=>  Location: PIN_L9,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_h[5]	=>  Location: PIN_N4,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_sec_h[6]	=>  Location: PIN_N9,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_l[0]	=>  Location: PIN_N13,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_l[1]	=>  Location: PIN_K11,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_l[2]	=>  Location: PIN_N10,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_l[3]	=>  Location: PIN_A11,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_l[4]	=>  Location: PIN_M9,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_l[5]	=>  Location: PIN_C8,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_l[6]	=>  Location: PIN_K13,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_h[0]	=>  Location: PIN_L5,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_h[1]	=>  Location: PIN_C6,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_h[2]	=>  Location: PIN_L7,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_h[3]	=>  Location: PIN_K8,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_h[4]	=>  Location: PIN_N11,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_h[5]	=>  Location: PIN_K9,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_min_h[6]	=>  Location: PIN_L12,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_hour[0]	=>  Location: PIN_H10,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_hour[1]	=>  Location: PIN_H12,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_hour[2]	=>  Location: PIN_G10,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_hour[3]	=>  Location: PIN_B8,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_hour[4]	=>  Location: PIN_G9,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_hour[5]	=>  Location: PIN_K12,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- out_hour[6]	=>  Location: PIN_J13,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- reset	=>  Location: PIN_J6,	 I/O Standard: 2.5 V,	 Current Strength: Default
-- clk	=>  Location: PIN_J7,	 I/O Standard: 2.5 V,	 Current Strength: Default


ARCHITECTURE structure OF timer IS
SIGNAL gnd : std_logic := '0';
SIGNAL vcc : std_logic := '1';
SIGNAL unknown : std_logic := 'X';
SIGNAL devoe : std_logic := '1';
SIGNAL devclrn : std_logic := '1';
SIGNAL devpor : std_logic := '1';
SIGNAL ww_devoe : std_logic;
SIGNAL ww_devclrn : std_logic;
SIGNAL ww_devpor : std_logic;
SIGNAL ww_reset : std_logic;
SIGNAL ww_clk : std_logic;
SIGNAL ww_out_sec_l : std_logic_vector(6 DOWNTO 0);
SIGNAL ww_out_sec_h : std_logic_vector(6 DOWNTO 0);
SIGNAL ww_out_min_l : std_logic_vector(6 DOWNTO 0);
SIGNAL ww_out_min_h : std_logic_vector(6 DOWNTO 0);
SIGNAL ww_out_hour : std_logic_vector(6 DOWNTO 0);
SIGNAL \sec2min|cout~clkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \min2hour|cout~clkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \sec_count|clk_out~clkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \reset~inputclkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \clk~inputclkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \show_hour|Mux7~0clkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \show_min_l|Mux7~0clkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \show_min_h|Mux7~0clkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \show_sec_h|Mux7~0clkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \show_sec_l|Mux7~0clkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \out_sec_l[0]~output_o\ : std_logic;
SIGNAL \out_sec_l[1]~output_o\ : std_logic;
SIGNAL \out_sec_l[2]~output_o\ : std_logic;
SIGNAL \out_sec_l[3]~output_o\ : std_logic;
SIGNAL \out_sec_l[4]~output_o\ : std_logic;
SIGNAL \out_sec_l[5]~output_o\ : std_logic;
SIGNAL \out_sec_l[6]~output_o\ : std_logic;
SIGNAL \out_sec_h[0]~output_o\ : std_logic;
SIGNAL \out_sec_h[1]~output_o\ : std_logic;
SIGNAL \out_sec_h[2]~output_o\ : std_logic;
SIGNAL \out_sec_h[3]~output_o\ : std_logic;
SIGNAL \out_sec_h[4]~output_o\ : std_logic;
SIGNAL \out_sec_h[5]~output_o\ : std_logic;
SIGNAL \out_sec_h[6]~output_o\ : std_logic;
SIGNAL \out_min_l[0]~output_o\ : std_logic;
SIGNAL \out_min_l[1]~output_o\ : std_logic;
SIGNAL \out_min_l[2]~output_o\ : std_logic;
SIGNAL \out_min_l[3]~output_o\ : std_logic;
SIGNAL \out_min_l[4]~output_o\ : std_logic;
SIGNAL \out_min_l[5]~output_o\ : std_logic;
SIGNAL \out_min_l[6]~output_o\ : std_logic;
SIGNAL \out_min_h[0]~output_o\ : std_logic;
SIGNAL \out_min_h[1]~output_o\ : std_logic;
SIGNAL \out_min_h[2]~output_o\ : std_logic;
SIGNAL \out_min_h[3]~output_o\ : std_logic;
SIGNAL \out_min_h[4]~output_o\ : std_logic;
SIGNAL \out_min_h[5]~output_o\ : std_logic;
SIGNAL \out_min_h[6]~output_o\ : std_logic;
SIGNAL \out_hour[0]~output_o\ : std_logic;
SIGNAL \out_hour[1]~output_o\ : std_logic;
SIGNAL \out_hour[2]~output_o\ : std_logic;
SIGNAL \out_hour[3]~output_o\ : std_logic;
SIGNAL \out_hour[4]~output_o\ : std_logic;
SIGNAL \out_hour[5]~output_o\ : std_logic;
SIGNAL \out_hour[6]~output_o\ : std_logic;
SIGNAL \clk~input_o\ : std_logic;
SIGNAL \clk~inputclkctrl_outclk\ : std_logic;
SIGNAL \reset~input_o\ : std_logic;
SIGNAL \sec_count|cnt[0]~32_combout\ : std_logic;
SIGNAL \sec_count|cnt[4]~34_combout\ : std_logic;
SIGNAL \sec_count|cnt[0]~33\ : std_logic;
SIGNAL \sec_count|cnt[1]~35_combout\ : std_logic;
SIGNAL \sec_count|cnt[1]~36\ : std_logic;
SIGNAL \sec_count|cnt[2]~37_combout\ : std_logic;
SIGNAL \sec_count|cnt[2]~38\ : std_logic;
SIGNAL \sec_count|cnt[3]~39_combout\ : std_logic;
SIGNAL \sec_count|cnt[3]~40\ : std_logic;
SIGNAL \sec_count|cnt[4]~41_combout\ : std_logic;
SIGNAL \sec_count|cnt[4]~42\ : std_logic;
SIGNAL \sec_count|cnt[5]~43_combout\ : std_logic;
SIGNAL \sec_count|cnt[5]~44\ : std_logic;
SIGNAL \sec_count|cnt[6]~45_combout\ : std_logic;
SIGNAL \sec_count|cnt[6]~46\ : std_logic;
SIGNAL \sec_count|cnt[7]~47_combout\ : std_logic;
SIGNAL \sec_count|cnt[7]~48\ : std_logic;
SIGNAL \sec_count|cnt[8]~49_combout\ : std_logic;
SIGNAL \sec_count|cnt[8]~50\ : std_logic;
SIGNAL \sec_count|cnt[9]~51_combout\ : std_logic;
SIGNAL \sec_count|cnt[9]~52\ : std_logic;
SIGNAL \sec_count|cnt[10]~53_combout\ : std_logic;
SIGNAL \sec_count|cnt[10]~54\ : std_logic;
SIGNAL \sec_count|cnt[11]~55_combout\ : std_logic;
SIGNAL \sec_count|cnt[11]~56\ : std_logic;
SIGNAL \sec_count|cnt[12]~57_combout\ : std_logic;
SIGNAL \sec_count|cnt[12]~58\ : std_logic;
SIGNAL \sec_count|cnt[13]~59_combout\ : std_logic;
SIGNAL \sec_count|cnt[13]~60\ : std_logic;
SIGNAL \sec_count|cnt[14]~61_combout\ : std_logic;
SIGNAL \sec_count|cnt[14]~62\ : std_logic;
SIGNAL \sec_count|cnt[15]~63_combout\ : std_logic;
SIGNAL \sec_count|cnt[15]~64\ : std_logic;
SIGNAL \sec_count|cnt[16]~65_combout\ : std_logic;
SIGNAL \sec_count|cnt[16]~66\ : std_logic;
SIGNAL \sec_count|cnt[17]~67_combout\ : std_logic;
SIGNAL \sec_count|cnt[17]~68\ : std_logic;
SIGNAL \sec_count|cnt[18]~69_combout\ : std_logic;
SIGNAL \sec_count|cnt[18]~70\ : std_logic;
SIGNAL \sec_count|cnt[19]~71_combout\ : std_logic;
SIGNAL \sec_count|cnt[19]~72\ : std_logic;
SIGNAL \sec_count|cnt[20]~73_combout\ : std_logic;
SIGNAL \sec_count|cnt[20]~74\ : std_logic;
SIGNAL \sec_count|cnt[21]~75_combout\ : std_logic;
SIGNAL \sec_count|cnt[21]~76\ : std_logic;
SIGNAL \sec_count|cnt[22]~77_combout\ : std_logic;
SIGNAL \sec_count|cnt[22]~78\ : std_logic;
SIGNAL \sec_count|cnt[23]~79_combout\ : std_logic;
SIGNAL \sec_count|cnt[23]~80\ : std_logic;
SIGNAL \sec_count|cnt[24]~81_combout\ : std_logic;
SIGNAL \sec_count|cnt[24]~82\ : std_logic;
SIGNAL \sec_count|cnt[25]~83_combout\ : std_logic;
SIGNAL \sec_count|cnt[25]~84\ : std_logic;
SIGNAL \sec_count|cnt[26]~85_combout\ : std_logic;
SIGNAL \sec_count|cnt[26]~86\ : std_logic;
SIGNAL \sec_count|cnt[27]~87_combout\ : std_logic;
SIGNAL \sec_count|Equal0~8_combout\ : std_logic;
SIGNAL \sec_count|Equal0~5_combout\ : std_logic;
SIGNAL \sec_count|Equal0~6_combout\ : std_logic;
SIGNAL \sec_count|Equal0~7_combout\ : std_logic;
SIGNAL \sec_count|cnt[27]~88\ : std_logic;
SIGNAL \sec_count|cnt[28]~89_combout\ : std_logic;
SIGNAL \sec_count|cnt[28]~90\ : std_logic;
SIGNAL \sec_count|cnt[29]~91_combout\ : std_logic;
SIGNAL \sec_count|cnt[29]~92\ : std_logic;
SIGNAL \sec_count|cnt[30]~93_combout\ : std_logic;
SIGNAL \sec_count|cnt[30]~94\ : std_logic;
SIGNAL \sec_count|cnt[31]~95_combout\ : std_logic;
SIGNAL \sec_count|Equal0~9_combout\ : std_logic;
SIGNAL \sec_count|Equal0~3_combout\ : std_logic;
SIGNAL \sec_count|Equal0~2_combout\ : std_logic;
SIGNAL \sec_count|Equal0~1_combout\ : std_logic;
SIGNAL \sec_count|Equal0~0_combout\ : std_logic;
SIGNAL \sec_count|Equal0~4_combout\ : std_logic;
SIGNAL \sec_count|Equal0~10_combout\ : std_logic;
SIGNAL \sec_count|clk_out~0_combout\ : std_logic;
SIGNAL \sec_count|clk_out~feeder_combout\ : std_logic;
SIGNAL \sec_count|clk_out~q\ : std_logic;
SIGNAL \sec_count|clk_out~clkctrl_outclk\ : std_logic;
SIGNAL \sec2min|qout[0]~7_combout\ : std_logic;
SIGNAL \reset~inputclkctrl_outclk\ : std_logic;
SIGNAL \sec2min|qout~0_combout\ : std_logic;
SIGNAL \sec2min|qout[2]~1_combout\ : std_logic;
SIGNAL \sec2min|qout~2_combout\ : std_logic;
SIGNAL \show_sec_l|Mux7~0_combout\ : std_logic;
SIGNAL \show_sec_l|Mux7~0clkctrl_outclk\ : std_logic;
SIGNAL \show_sec_l|Mux0~0_combout\ : std_logic;
SIGNAL \show_sec_l|Mux1~0_combout\ : std_logic;
SIGNAL \show_sec_l|Mux2~0_combout\ : std_logic;
SIGNAL \show_sec_l|Mux3~0_combout\ : std_logic;
SIGNAL \show_sec_l|Mux4~0_combout\ : std_logic;
SIGNAL \show_sec_l|Mux5~0_combout\ : std_logic;
SIGNAL \show_sec_l|Mux6~0_combout\ : std_logic;
SIGNAL \sec2min|Equal0~0_combout\ : std_logic;
SIGNAL \sec2min|qout[4]~8_combout\ : std_logic;
SIGNAL \sec2min|qout~3_combout\ : std_logic;
SIGNAL \sec2min|qout[7]~5_combout\ : std_logic;
SIGNAL \sec2min|qout[7]~6_combout\ : std_logic;
SIGNAL \sec2min|qout~4_combout\ : std_logic;
SIGNAL \show_sec_h|Mux7~0_combout\ : std_logic;
SIGNAL \show_sec_h|Mux7~0clkctrl_outclk\ : std_logic;
SIGNAL \show_sec_h|Mux0~0_combout\ : std_logic;
SIGNAL \show_sec_h|Mux1~0_combout\ : std_logic;
SIGNAL \show_sec_h|Mux2~0_combout\ : std_logic;
SIGNAL \show_sec_h|Mux3~0_combout\ : std_logic;
SIGNAL \show_sec_h|Mux4~0_combout\ : std_logic;
SIGNAL \show_sec_h|Mux5~0_combout\ : std_logic;
SIGNAL \show_sec_h|Mux6~0_combout\ : std_logic;
SIGNAL \sec2min|Equal1~0_combout\ : std_logic;
SIGNAL \sec2min|Equal2~0_combout\ : std_logic;
SIGNAL \sec2min|cout~q\ : std_logic;
SIGNAL \sec2min|cout~clkctrl_outclk\ : std_logic;
SIGNAL \min2hour|qout[0]~7_combout\ : std_logic;
SIGNAL \min2hour|qout~2_combout\ : std_logic;
SIGNAL \min2hour|qout~0_combout\ : std_logic;
SIGNAL \min2hour|qout[2]~1_combout\ : std_logic;
SIGNAL \show_min_l|Mux7~0_combout\ : std_logic;
SIGNAL \show_min_l|Mux7~0clkctrl_outclk\ : std_logic;
SIGNAL \show_min_l|Mux0~0_combout\ : std_logic;
SIGNAL \show_min_l|Mux1~0_combout\ : std_logic;
SIGNAL \show_min_l|Mux2~0_combout\ : std_logic;
SIGNAL \show_min_l|Mux3~0_combout\ : std_logic;
SIGNAL \show_min_l|Mux4~0_combout\ : std_logic;
SIGNAL \show_min_l|Mux5~0_combout\ : std_logic;
SIGNAL \show_min_l|Mux6~0_combout\ : std_logic;
SIGNAL \min2hour|qout[4]~8_combout\ : std_logic;
SIGNAL \min2hour|Equal0~0_combout\ : std_logic;
SIGNAL \min2hour|qout~3_combout\ : std_logic;
SIGNAL \min2hour|qout[7]~5_combout\ : std_logic;
SIGNAL \min2hour|qout[7]~6_combout\ : std_logic;
SIGNAL \min2hour|qout~4_combout\ : std_logic;
SIGNAL \show_min_h|Mux7~0_combout\ : std_logic;
SIGNAL \show_min_h|Mux7~0clkctrl_outclk\ : std_logic;
SIGNAL \show_min_h|Mux0~0_combout\ : std_logic;
SIGNAL \show_min_h|Mux1~0_combout\ : std_logic;
SIGNAL \show_min_h|Mux2~0_combout\ : std_logic;
SIGNAL \show_min_h|Mux3~0_combout\ : std_logic;
SIGNAL \show_min_h|Mux4~0_combout\ : std_logic;
SIGNAL \show_min_h|Mux5~0_combout\ : std_logic;
SIGNAL \show_min_h|Mux6~0_combout\ : std_logic;
SIGNAL \min2hour|Equal1~0_combout\ : std_logic;
SIGNAL \min2hour|Equal2~0_combout\ : std_logic;
SIGNAL \min2hour|cout~q\ : std_logic;
SIGNAL \min2hour|cout~clkctrl_outclk\ : std_logic;
SIGNAL \hour~0_combout\ : std_logic;
SIGNAL \hour~1_combout\ : std_logic;
SIGNAL \hour[1]~feeder_combout\ : std_logic;
SIGNAL \Add0~0_combout\ : std_logic;
SIGNAL \Add0~1_combout\ : std_logic;
SIGNAL \show_hour|Mux7~0_combout\ : std_logic;
SIGNAL \show_hour|Mux7~0clkctrl_outclk\ : std_logic;
SIGNAL \show_hour|Mux0~0_combout\ : std_logic;
SIGNAL \show_hour|Mux1~0_combout\ : std_logic;
SIGNAL \show_hour|Mux2~0_combout\ : std_logic;
SIGNAL \show_hour|Mux3~0_combout\ : std_logic;
SIGNAL \show_hour|Mux4~0_combout\ : std_logic;
SIGNAL \show_hour|Mux5~0_combout\ : std_logic;
SIGNAL \show_hour|Mux6~0_combout\ : std_logic;
SIGNAL \min2hour|qout\ : std_logic_vector(7 DOWNTO 0);
SIGNAL \show_sec_h|decodeout\ : std_logic_vector(6 DOWNTO 0);
SIGNAL \show_min_l|decodeout\ : std_logic_vector(6 DOWNTO 0);
SIGNAL \show_min_h|decodeout\ : std_logic_vector(6 DOWNTO 0);
SIGNAL \show_hour|decodeout\ : std_logic_vector(6 DOWNTO 0);
SIGNAL hour : std_logic_vector(3 DOWNTO 0);
SIGNAL \sec_count|cnt\ : std_logic_vector(31 DOWNTO 0);
SIGNAL \sec2min|qout\ : std_logic_vector(7 DOWNTO 0);
SIGNAL \show_sec_l|decodeout\ : std_logic_vector(6 DOWNTO 0);
SIGNAL \ALT_INV_reset~inputclkctrl_outclk\ : std_logic;
SIGNAL \min2hour|ALT_INV_cout~clkctrl_outclk\ : std_logic;
SIGNAL \ALT_INV_reset~input_o\ : std_logic;
SIGNAL \min2hour|ALT_INV_cout~q\ : std_logic;

BEGIN

ww_reset <= reset;
ww_clk <= clk;
out_sec_l <= ww_out_sec_l;
out_sec_h <= ww_out_sec_h;
out_min_l <= ww_out_min_l;
out_min_h <= ww_out_min_h;
out_hour <= ww_out_hour;
ww_devoe <= devoe;
ww_devclrn <= devclrn;
ww_devpor <= devpor;

\sec2min|cout~clkctrl_INCLK_bus\ <= (vcc & vcc & vcc & \sec2min|cout~q\);

\min2hour|cout~clkctrl_INCLK_bus\ <= (vcc & vcc & vcc & \min2hour|cout~q\);

\sec_count|clk_out~clkctrl_INCLK_bus\ <= (vcc & vcc & vcc & \sec_count|clk_out~q\);

\reset~inputclkctrl_INCLK_bus\ <= (vcc & vcc & vcc & \reset~input_o\);

\clk~inputclkctrl_INCLK_bus\ <= (vcc & vcc & vcc & \clk~input_o\);

\show_hour|Mux7~0clkctrl_INCLK_bus\ <= (vcc & vcc & vcc & \show_hour|Mux7~0_combout\);

\show_min_l|Mux7~0clkctrl_INCLK_bus\ <= (vcc & vcc & vcc & \show_min_l|Mux7~0_combout\);

\show_min_h|Mux7~0clkctrl_INCLK_bus\ <= (vcc & vcc & vcc & \show_min_h|Mux7~0_combout\);

\show_sec_h|Mux7~0clkctrl_INCLK_bus\ <= (vcc & vcc & vcc & \show_sec_h|Mux7~0_combout\);

\show_sec_l|Mux7~0clkctrl_INCLK_bus\ <= (vcc & vcc & vcc & \show_sec_l|Mux7~0_combout\);
\ALT_INV_reset~inputclkctrl_outclk\ <= NOT \reset~inputclkctrl_outclk\;
\min2hour|ALT_INV_cout~clkctrl_outclk\ <= NOT \min2hour|cout~clkctrl_outclk\;
\ALT_INV_reset~input_o\ <= NOT \reset~input_o\;
\min2hour|ALT_INV_cout~q\ <= NOT \min2hour|cout~q\;

-- Location: IOOBUF_X12_Y0_N2
\out_sec_l[0]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_l|decodeout\(0),
	devoe => ww_devoe,
	o => \out_sec_l[0]~output_o\);

-- Location: IOOBUF_X20_Y31_N9
\out_sec_l[1]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_l|decodeout\(1),
	devoe => ww_devoe,
	o => \out_sec_l[1]~output_o\);

-- Location: IOOBUF_X8_Y0_N2
\out_sec_l[2]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_l|decodeout\(2),
	devoe => ww_devoe,
	o => \out_sec_l[2]~output_o\);

-- Location: IOOBUF_X33_Y12_N9
\out_sec_l[3]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_l|decodeout\(3),
	devoe => ww_devoe,
	o => \out_sec_l[3]~output_o\);

-- Location: IOOBUF_X12_Y31_N2
\out_sec_l[4]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_l|decodeout\(4),
	devoe => ww_devoe,
	o => \out_sec_l[4]~output_o\);

-- Location: IOOBUF_X12_Y31_N9
\out_sec_l[5]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_l|decodeout\(5),
	devoe => ww_devoe,
	o => \out_sec_l[5]~output_o\);

-- Location: IOOBUF_X10_Y31_N2
\out_sec_l[6]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_l|decodeout\(6),
	devoe => ww_devoe,
	o => \out_sec_l[6]~output_o\);

-- Location: IOOBUF_X8_Y0_N9
\out_sec_h[0]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_h|decodeout\(0),
	devoe => ww_devoe,
	o => \out_sec_h[0]~output_o\);

-- Location: IOOBUF_X12_Y0_N9
\out_sec_h[1]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_h|decodeout\(1),
	devoe => ww_devoe,
	o => \out_sec_h[1]~output_o\);

-- Location: IOOBUF_X20_Y0_N9
\out_sec_h[2]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_h|decodeout\(2),
	devoe => ww_devoe,
	o => \out_sec_h[2]~output_o\);

-- Location: IOOBUF_X14_Y31_N9
\out_sec_h[3]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_h|decodeout\(3),
	devoe => ww_devoe,
	o => \out_sec_h[3]~output_o\);

-- Location: IOOBUF_X24_Y0_N9
\out_sec_h[4]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_h|decodeout\(4),
	devoe => ww_devoe,
	o => \out_sec_h[4]~output_o\);

-- Location: IOOBUF_X10_Y0_N9
\out_sec_h[5]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_h|decodeout\(5),
	devoe => ww_devoe,
	o => \out_sec_h[5]~output_o\);

-- Location: IOOBUF_X20_Y0_N2
\out_sec_h[6]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_sec_h|decodeout\(6),
	devoe => ww_devoe,
	o => \out_sec_h[6]~output_o\);

-- Location: IOOBUF_X33_Y10_N9
\out_min_l[0]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_l|decodeout\(0),
	devoe => ww_devoe,
	o => \out_min_l[0]~output_o\);

-- Location: IOOBUF_X33_Y11_N2
\out_min_l[1]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_l|decodeout\(1),
	devoe => ww_devoe,
	o => \out_min_l[1]~output_o\);

-- Location: IOOBUF_X26_Y0_N9
\out_min_l[2]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_l|decodeout\(2),
	devoe => ww_devoe,
	o => \out_min_l[2]~output_o\);

-- Location: IOOBUF_X20_Y31_N2
\out_min_l[3]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_l|decodeout\(3),
	devoe => ww_devoe,
	o => \out_min_l[3]~output_o\);

-- Location: IOOBUF_X24_Y0_N2
\out_min_l[4]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_l|decodeout\(4),
	devoe => ww_devoe,
	o => \out_min_l[4]~output_o\);

-- Location: IOOBUF_X22_Y31_N9
\out_min_l[5]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_l|decodeout\(5),
	devoe => ww_devoe,
	o => \out_min_l[5]~output_o\);

-- Location: IOOBUF_X33_Y15_N2
\out_min_l[6]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_l|decodeout\(6),
	devoe => ww_devoe,
	o => \out_min_l[6]~output_o\);

-- Location: IOOBUF_X14_Y0_N9
\out_min_h[0]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_h|decodeout\(0),
	devoe => ww_devoe,
	o => \out_min_h[0]~output_o\);

-- Location: IOOBUF_X14_Y31_N2
\out_min_h[1]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_h|decodeout\(1),
	devoe => ww_devoe,
	o => \out_min_h[1]~output_o\);

-- Location: IOOBUF_X14_Y0_N2
\out_min_h[2]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_h|decodeout\(2),
	devoe => ww_devoe,
	o => \out_min_h[2]~output_o\);

-- Location: IOOBUF_X22_Y0_N9
\out_min_h[3]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_h|decodeout\(3),
	devoe => ww_devoe,
	o => \out_min_h[3]~output_o\);

-- Location: IOOBUF_X26_Y0_N2
\out_min_h[4]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_h|decodeout\(4),
	devoe => ww_devoe,
	o => \out_min_h[4]~output_o\);

-- Location: IOOBUF_X22_Y0_N2
\out_min_h[5]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_h|decodeout\(5),
	devoe => ww_devoe,
	o => \out_min_h[5]~output_o\);

-- Location: IOOBUF_X33_Y12_N2
\out_min_h[6]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_min_h|decodeout\(6),
	devoe => ww_devoe,
	o => \out_min_h[6]~output_o\);

-- Location: IOOBUF_X33_Y14_N2
\out_hour[0]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_hour|decodeout\(0),
	devoe => ww_devoe,
	o => \out_hour[0]~output_o\);

-- Location: IOOBUF_X33_Y14_N9
\out_hour[1]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_hour|decodeout\(1),
	devoe => ww_devoe,
	o => \out_hour[1]~output_o\);

-- Location: IOOBUF_X33_Y22_N9
\out_hour[2]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_hour|decodeout\(2),
	devoe => ww_devoe,
	o => \out_hour[2]~output_o\);

-- Location: IOOBUF_X22_Y31_N2
\out_hour[3]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_hour|decodeout\(3),
	devoe => ww_devoe,
	o => \out_hour[3]~output_o\);

-- Location: IOOBUF_X33_Y22_N2
\out_hour[4]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_hour|decodeout\(4),
	devoe => ww_devoe,
	o => \out_hour[4]~output_o\);

-- Location: IOOBUF_X33_Y11_N9
\out_hour[5]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_hour|decodeout\(5),
	devoe => ww_devoe,
	o => \out_hour[5]~output_o\);

-- Location: IOOBUF_X33_Y15_N9
\out_hour[6]~output\ : cycloneiv_io_obuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	open_drain_output => "false")
-- pragma translate_on
PORT MAP (
	i => \show_hour|decodeout\(6),
	devoe => ww_devoe,
	o => \out_hour[6]~output_o\);

-- Location: IOIBUF_X16_Y0_N15
\clk~input\ : cycloneiv_io_ibuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	simulate_z_as => "z")
-- pragma translate_on
PORT MAP (
	i => ww_clk,
	o => \clk~input_o\);

-- Location: CLKCTRL_G17
\clk~inputclkctrl\ : cycloneiv_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \clk~inputclkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \clk~inputclkctrl_outclk\);

-- Location: IOIBUF_X16_Y0_N22
\reset~input\ : cycloneiv_io_ibuf
-- pragma translate_off
GENERIC MAP (
	bus_hold => "false",
	simulate_z_as => "z")
-- pragma translate_on
PORT MAP (
	i => ww_reset,
	o => \reset~input_o\);

-- Location: LCCOMB_X4_Y2_N0
\sec_count|cnt[0]~32\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[0]~32_combout\ = \sec_count|cnt\(0) $ (VCC)
-- \sec_count|cnt[0]~33\ = CARRY(\sec_count|cnt\(0))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011001111001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(0),
	datad => VCC,
	combout => \sec_count|cnt[0]~32_combout\,
	cout => \sec_count|cnt[0]~33\);

-- Location: LCCOMB_X5_Y1_N0
\sec_count|cnt[4]~34\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[4]~34_combout\ = (\reset~input_o\) # (!\sec_count|Equal0~10_combout\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111000011111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datac => \reset~input_o\,
	datad => \sec_count|Equal0~10_combout\,
	combout => \sec_count|cnt[4]~34_combout\);

-- Location: FF_X4_Y2_N1
\sec_count|cnt[0]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[0]~32_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(0));

-- Location: LCCOMB_X4_Y2_N2
\sec_count|cnt[1]~35\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[1]~35_combout\ = (\sec_count|cnt\(1) & (!\sec_count|cnt[0]~33\)) # (!\sec_count|cnt\(1) & ((\sec_count|cnt[0]~33\) # (GND)))
-- \sec_count|cnt[1]~36\ = CARRY((!\sec_count|cnt[0]~33\) # (!\sec_count|cnt\(1)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011110000111111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(1),
	datad => VCC,
	cin => \sec_count|cnt[0]~33\,
	combout => \sec_count|cnt[1]~35_combout\,
	cout => \sec_count|cnt[1]~36\);

-- Location: FF_X4_Y2_N3
\sec_count|cnt[1]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[1]~35_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(1));

-- Location: LCCOMB_X4_Y2_N4
\sec_count|cnt[2]~37\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[2]~37_combout\ = (\sec_count|cnt\(2) & (\sec_count|cnt[1]~36\ $ (GND))) # (!\sec_count|cnt\(2) & (!\sec_count|cnt[1]~36\ & VCC))
-- \sec_count|cnt[2]~38\ = CARRY((\sec_count|cnt\(2) & !\sec_count|cnt[1]~36\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(2),
	datad => VCC,
	cin => \sec_count|cnt[1]~36\,
	combout => \sec_count|cnt[2]~37_combout\,
	cout => \sec_count|cnt[2]~38\);

-- Location: FF_X4_Y2_N5
\sec_count|cnt[2]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[2]~37_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(2));

-- Location: LCCOMB_X4_Y2_N6
\sec_count|cnt[3]~39\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[3]~39_combout\ = (\sec_count|cnt\(3) & (!\sec_count|cnt[2]~38\)) # (!\sec_count|cnt\(3) & ((\sec_count|cnt[2]~38\) # (GND)))
-- \sec_count|cnt[3]~40\ = CARRY((!\sec_count|cnt[2]~38\) # (!\sec_count|cnt\(3)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101001011111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(3),
	datad => VCC,
	cin => \sec_count|cnt[2]~38\,
	combout => \sec_count|cnt[3]~39_combout\,
	cout => \sec_count|cnt[3]~40\);

-- Location: FF_X4_Y2_N7
\sec_count|cnt[3]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[3]~39_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(3));

-- Location: LCCOMB_X4_Y2_N8
\sec_count|cnt[4]~41\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[4]~41_combout\ = (\sec_count|cnt\(4) & (\sec_count|cnt[3]~40\ $ (GND))) # (!\sec_count|cnt\(4) & (!\sec_count|cnt[3]~40\ & VCC))
-- \sec_count|cnt[4]~42\ = CARRY((\sec_count|cnt\(4) & !\sec_count|cnt[3]~40\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(4),
	datad => VCC,
	cin => \sec_count|cnt[3]~40\,
	combout => \sec_count|cnt[4]~41_combout\,
	cout => \sec_count|cnt[4]~42\);

-- Location: FF_X4_Y2_N9
\sec_count|cnt[4]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[4]~41_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(4));

-- Location: LCCOMB_X4_Y2_N10
\sec_count|cnt[5]~43\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[5]~43_combout\ = (\sec_count|cnt\(5) & (!\sec_count|cnt[4]~42\)) # (!\sec_count|cnt\(5) & ((\sec_count|cnt[4]~42\) # (GND)))
-- \sec_count|cnt[5]~44\ = CARRY((!\sec_count|cnt[4]~42\) # (!\sec_count|cnt\(5)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101001011111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(5),
	datad => VCC,
	cin => \sec_count|cnt[4]~42\,
	combout => \sec_count|cnt[5]~43_combout\,
	cout => \sec_count|cnt[5]~44\);

-- Location: FF_X4_Y2_N11
\sec_count|cnt[5]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[5]~43_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(5));

-- Location: LCCOMB_X4_Y2_N12
\sec_count|cnt[6]~45\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[6]~45_combout\ = (\sec_count|cnt\(6) & (\sec_count|cnt[5]~44\ $ (GND))) # (!\sec_count|cnt\(6) & (!\sec_count|cnt[5]~44\ & VCC))
-- \sec_count|cnt[6]~46\ = CARRY((\sec_count|cnt\(6) & !\sec_count|cnt[5]~44\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1010010100001010",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(6),
	datad => VCC,
	cin => \sec_count|cnt[5]~44\,
	combout => \sec_count|cnt[6]~45_combout\,
	cout => \sec_count|cnt[6]~46\);

-- Location: FF_X4_Y2_N13
\sec_count|cnt[6]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[6]~45_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(6));

-- Location: LCCOMB_X4_Y2_N14
\sec_count|cnt[7]~47\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[7]~47_combout\ = (\sec_count|cnt\(7) & (!\sec_count|cnt[6]~46\)) # (!\sec_count|cnt\(7) & ((\sec_count|cnt[6]~46\) # (GND)))
-- \sec_count|cnt[7]~48\ = CARRY((!\sec_count|cnt[6]~46\) # (!\sec_count|cnt\(7)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011110000111111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(7),
	datad => VCC,
	cin => \sec_count|cnt[6]~46\,
	combout => \sec_count|cnt[7]~47_combout\,
	cout => \sec_count|cnt[7]~48\);

-- Location: FF_X4_Y2_N15
\sec_count|cnt[7]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[7]~47_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(7));

-- Location: LCCOMB_X4_Y2_N16
\sec_count|cnt[8]~49\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[8]~49_combout\ = (\sec_count|cnt\(8) & (\sec_count|cnt[7]~48\ $ (GND))) # (!\sec_count|cnt\(8) & (!\sec_count|cnt[7]~48\ & VCC))
-- \sec_count|cnt[8]~50\ = CARRY((\sec_count|cnt\(8) & !\sec_count|cnt[7]~48\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(8),
	datad => VCC,
	cin => \sec_count|cnt[7]~48\,
	combout => \sec_count|cnt[8]~49_combout\,
	cout => \sec_count|cnt[8]~50\);

-- Location: FF_X4_Y2_N17
\sec_count|cnt[8]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[8]~49_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(8));

-- Location: LCCOMB_X4_Y2_N18
\sec_count|cnt[9]~51\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[9]~51_combout\ = (\sec_count|cnt\(9) & (!\sec_count|cnt[8]~50\)) # (!\sec_count|cnt\(9) & ((\sec_count|cnt[8]~50\) # (GND)))
-- \sec_count|cnt[9]~52\ = CARRY((!\sec_count|cnt[8]~50\) # (!\sec_count|cnt\(9)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011110000111111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(9),
	datad => VCC,
	cin => \sec_count|cnt[8]~50\,
	combout => \sec_count|cnt[9]~51_combout\,
	cout => \sec_count|cnt[9]~52\);

-- Location: FF_X4_Y2_N19
\sec_count|cnt[9]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[9]~51_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(9));

-- Location: LCCOMB_X4_Y2_N20
\sec_count|cnt[10]~53\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[10]~53_combout\ = (\sec_count|cnt\(10) & (\sec_count|cnt[9]~52\ $ (GND))) # (!\sec_count|cnt\(10) & (!\sec_count|cnt[9]~52\ & VCC))
-- \sec_count|cnt[10]~54\ = CARRY((\sec_count|cnt\(10) & !\sec_count|cnt[9]~52\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(10),
	datad => VCC,
	cin => \sec_count|cnt[9]~52\,
	combout => \sec_count|cnt[10]~53_combout\,
	cout => \sec_count|cnt[10]~54\);

-- Location: FF_X4_Y2_N21
\sec_count|cnt[10]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[10]~53_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(10));

-- Location: LCCOMB_X4_Y2_N22
\sec_count|cnt[11]~55\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[11]~55_combout\ = (\sec_count|cnt\(11) & (!\sec_count|cnt[10]~54\)) # (!\sec_count|cnt\(11) & ((\sec_count|cnt[10]~54\) # (GND)))
-- \sec_count|cnt[11]~56\ = CARRY((!\sec_count|cnt[10]~54\) # (!\sec_count|cnt\(11)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101001011111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(11),
	datad => VCC,
	cin => \sec_count|cnt[10]~54\,
	combout => \sec_count|cnt[11]~55_combout\,
	cout => \sec_count|cnt[11]~56\);

-- Location: FF_X4_Y2_N23
\sec_count|cnt[11]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[11]~55_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(11));

-- Location: LCCOMB_X4_Y2_N24
\sec_count|cnt[12]~57\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[12]~57_combout\ = (\sec_count|cnt\(12) & (\sec_count|cnt[11]~56\ $ (GND))) # (!\sec_count|cnt\(12) & (!\sec_count|cnt[11]~56\ & VCC))
-- \sec_count|cnt[12]~58\ = CARRY((\sec_count|cnt\(12) & !\sec_count|cnt[11]~56\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(12),
	datad => VCC,
	cin => \sec_count|cnt[11]~56\,
	combout => \sec_count|cnt[12]~57_combout\,
	cout => \sec_count|cnt[12]~58\);

-- Location: FF_X4_Y2_N25
\sec_count|cnt[12]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[12]~57_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(12));

-- Location: LCCOMB_X4_Y2_N26
\sec_count|cnt[13]~59\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[13]~59_combout\ = (\sec_count|cnt\(13) & (!\sec_count|cnt[12]~58\)) # (!\sec_count|cnt\(13) & ((\sec_count|cnt[12]~58\) # (GND)))
-- \sec_count|cnt[13]~60\ = CARRY((!\sec_count|cnt[12]~58\) # (!\sec_count|cnt\(13)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101001011111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(13),
	datad => VCC,
	cin => \sec_count|cnt[12]~58\,
	combout => \sec_count|cnt[13]~59_combout\,
	cout => \sec_count|cnt[13]~60\);

-- Location: FF_X4_Y2_N27
\sec_count|cnt[13]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[13]~59_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(13));

-- Location: LCCOMB_X4_Y2_N28
\sec_count|cnt[14]~61\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[14]~61_combout\ = (\sec_count|cnt\(14) & (\sec_count|cnt[13]~60\ $ (GND))) # (!\sec_count|cnt\(14) & (!\sec_count|cnt[13]~60\ & VCC))
-- \sec_count|cnt[14]~62\ = CARRY((\sec_count|cnt\(14) & !\sec_count|cnt[13]~60\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(14),
	datad => VCC,
	cin => \sec_count|cnt[13]~60\,
	combout => \sec_count|cnt[14]~61_combout\,
	cout => \sec_count|cnt[14]~62\);

-- Location: FF_X4_Y2_N29
\sec_count|cnt[14]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[14]~61_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(14));

-- Location: LCCOMB_X4_Y2_N30
\sec_count|cnt[15]~63\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[15]~63_combout\ = (\sec_count|cnt\(15) & (!\sec_count|cnt[14]~62\)) # (!\sec_count|cnt\(15) & ((\sec_count|cnt[14]~62\) # (GND)))
-- \sec_count|cnt[15]~64\ = CARRY((!\sec_count|cnt[14]~62\) # (!\sec_count|cnt\(15)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101001011111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(15),
	datad => VCC,
	cin => \sec_count|cnt[14]~62\,
	combout => \sec_count|cnt[15]~63_combout\,
	cout => \sec_count|cnt[15]~64\);

-- Location: FF_X4_Y2_N31
\sec_count|cnt[15]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[15]~63_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(15));

-- Location: LCCOMB_X4_Y1_N0
\sec_count|cnt[16]~65\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[16]~65_combout\ = (\sec_count|cnt\(16) & (\sec_count|cnt[15]~64\ $ (GND))) # (!\sec_count|cnt\(16) & (!\sec_count|cnt[15]~64\ & VCC))
-- \sec_count|cnt[16]~66\ = CARRY((\sec_count|cnt\(16) & !\sec_count|cnt[15]~64\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(16),
	datad => VCC,
	cin => \sec_count|cnt[15]~64\,
	combout => \sec_count|cnt[16]~65_combout\,
	cout => \sec_count|cnt[16]~66\);

-- Location: FF_X4_Y1_N1
\sec_count|cnt[16]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[16]~65_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(16));

-- Location: LCCOMB_X4_Y1_N2
\sec_count|cnt[17]~67\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[17]~67_combout\ = (\sec_count|cnt\(17) & (!\sec_count|cnt[16]~66\)) # (!\sec_count|cnt\(17) & ((\sec_count|cnt[16]~66\) # (GND)))
-- \sec_count|cnt[17]~68\ = CARRY((!\sec_count|cnt[16]~66\) # (!\sec_count|cnt\(17)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011110000111111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(17),
	datad => VCC,
	cin => \sec_count|cnt[16]~66\,
	combout => \sec_count|cnt[17]~67_combout\,
	cout => \sec_count|cnt[17]~68\);

-- Location: FF_X4_Y1_N3
\sec_count|cnt[17]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[17]~67_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(17));

-- Location: LCCOMB_X4_Y1_N4
\sec_count|cnt[18]~69\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[18]~69_combout\ = (\sec_count|cnt\(18) & (\sec_count|cnt[17]~68\ $ (GND))) # (!\sec_count|cnt\(18) & (!\sec_count|cnt[17]~68\ & VCC))
-- \sec_count|cnt[18]~70\ = CARRY((\sec_count|cnt\(18) & !\sec_count|cnt[17]~68\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(18),
	datad => VCC,
	cin => \sec_count|cnt[17]~68\,
	combout => \sec_count|cnt[18]~69_combout\,
	cout => \sec_count|cnt[18]~70\);

-- Location: FF_X4_Y1_N5
\sec_count|cnt[18]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[18]~69_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(18));

-- Location: LCCOMB_X4_Y1_N6
\sec_count|cnt[19]~71\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[19]~71_combout\ = (\sec_count|cnt\(19) & (!\sec_count|cnt[18]~70\)) # (!\sec_count|cnt\(19) & ((\sec_count|cnt[18]~70\) # (GND)))
-- \sec_count|cnt[19]~72\ = CARRY((!\sec_count|cnt[18]~70\) # (!\sec_count|cnt\(19)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101001011111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(19),
	datad => VCC,
	cin => \sec_count|cnt[18]~70\,
	combout => \sec_count|cnt[19]~71_combout\,
	cout => \sec_count|cnt[19]~72\);

-- Location: FF_X4_Y1_N7
\sec_count|cnt[19]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[19]~71_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(19));

-- Location: LCCOMB_X4_Y1_N8
\sec_count|cnt[20]~73\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[20]~73_combout\ = (\sec_count|cnt\(20) & (\sec_count|cnt[19]~72\ $ (GND))) # (!\sec_count|cnt\(20) & (!\sec_count|cnt[19]~72\ & VCC))
-- \sec_count|cnt[20]~74\ = CARRY((\sec_count|cnt\(20) & !\sec_count|cnt[19]~72\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(20),
	datad => VCC,
	cin => \sec_count|cnt[19]~72\,
	combout => \sec_count|cnt[20]~73_combout\,
	cout => \sec_count|cnt[20]~74\);

-- Location: FF_X4_Y1_N9
\sec_count|cnt[20]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[20]~73_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(20));

-- Location: LCCOMB_X4_Y1_N10
\sec_count|cnt[21]~75\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[21]~75_combout\ = (\sec_count|cnt\(21) & (!\sec_count|cnt[20]~74\)) # (!\sec_count|cnt\(21) & ((\sec_count|cnt[20]~74\) # (GND)))
-- \sec_count|cnt[21]~76\ = CARRY((!\sec_count|cnt[20]~74\) # (!\sec_count|cnt\(21)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101001011111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(21),
	datad => VCC,
	cin => \sec_count|cnt[20]~74\,
	combout => \sec_count|cnt[21]~75_combout\,
	cout => \sec_count|cnt[21]~76\);

-- Location: FF_X4_Y1_N11
\sec_count|cnt[21]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[21]~75_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(21));

-- Location: LCCOMB_X4_Y1_N12
\sec_count|cnt[22]~77\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[22]~77_combout\ = (\sec_count|cnt\(22) & (\sec_count|cnt[21]~76\ $ (GND))) # (!\sec_count|cnt\(22) & (!\sec_count|cnt[21]~76\ & VCC))
-- \sec_count|cnt[22]~78\ = CARRY((\sec_count|cnt\(22) & !\sec_count|cnt[21]~76\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1010010100001010",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(22),
	datad => VCC,
	cin => \sec_count|cnt[21]~76\,
	combout => \sec_count|cnt[22]~77_combout\,
	cout => \sec_count|cnt[22]~78\);

-- Location: FF_X4_Y1_N13
\sec_count|cnt[22]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[22]~77_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(22));

-- Location: LCCOMB_X4_Y1_N14
\sec_count|cnt[23]~79\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[23]~79_combout\ = (\sec_count|cnt\(23) & (!\sec_count|cnt[22]~78\)) # (!\sec_count|cnt\(23) & ((\sec_count|cnt[22]~78\) # (GND)))
-- \sec_count|cnt[23]~80\ = CARRY((!\sec_count|cnt[22]~78\) # (!\sec_count|cnt\(23)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011110000111111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(23),
	datad => VCC,
	cin => \sec_count|cnt[22]~78\,
	combout => \sec_count|cnt[23]~79_combout\,
	cout => \sec_count|cnt[23]~80\);

-- Location: FF_X4_Y1_N15
\sec_count|cnt[23]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[23]~79_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(23));

-- Location: LCCOMB_X4_Y1_N16
\sec_count|cnt[24]~81\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[24]~81_combout\ = (\sec_count|cnt\(24) & (\sec_count|cnt[23]~80\ $ (GND))) # (!\sec_count|cnt\(24) & (!\sec_count|cnt[23]~80\ & VCC))
-- \sec_count|cnt[24]~82\ = CARRY((\sec_count|cnt\(24) & !\sec_count|cnt[23]~80\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(24),
	datad => VCC,
	cin => \sec_count|cnt[23]~80\,
	combout => \sec_count|cnt[24]~81_combout\,
	cout => \sec_count|cnt[24]~82\);

-- Location: FF_X4_Y1_N17
\sec_count|cnt[24]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[24]~81_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(24));

-- Location: LCCOMB_X4_Y1_N18
\sec_count|cnt[25]~83\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[25]~83_combout\ = (\sec_count|cnt\(25) & (!\sec_count|cnt[24]~82\)) # (!\sec_count|cnt\(25) & ((\sec_count|cnt[24]~82\) # (GND)))
-- \sec_count|cnt[25]~84\ = CARRY((!\sec_count|cnt[24]~82\) # (!\sec_count|cnt\(25)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011110000111111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(25),
	datad => VCC,
	cin => \sec_count|cnt[24]~82\,
	combout => \sec_count|cnt[25]~83_combout\,
	cout => \sec_count|cnt[25]~84\);

-- Location: FF_X4_Y1_N19
\sec_count|cnt[25]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[25]~83_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(25));

-- Location: LCCOMB_X4_Y1_N20
\sec_count|cnt[26]~85\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[26]~85_combout\ = (\sec_count|cnt\(26) & (\sec_count|cnt[25]~84\ $ (GND))) # (!\sec_count|cnt\(26) & (!\sec_count|cnt[25]~84\ & VCC))
-- \sec_count|cnt[26]~86\ = CARRY((\sec_count|cnt\(26) & !\sec_count|cnt[25]~84\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(26),
	datad => VCC,
	cin => \sec_count|cnt[25]~84\,
	combout => \sec_count|cnt[26]~85_combout\,
	cout => \sec_count|cnt[26]~86\);

-- Location: FF_X4_Y1_N21
\sec_count|cnt[26]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[26]~85_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(26));

-- Location: LCCOMB_X4_Y1_N22
\sec_count|cnt[27]~87\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[27]~87_combout\ = (\sec_count|cnt\(27) & (!\sec_count|cnt[26]~86\)) # (!\sec_count|cnt\(27) & ((\sec_count|cnt[26]~86\) # (GND)))
-- \sec_count|cnt[27]~88\ = CARRY((!\sec_count|cnt[26]~86\) # (!\sec_count|cnt\(27)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101001011111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(27),
	datad => VCC,
	cin => \sec_count|cnt[26]~86\,
	combout => \sec_count|cnt[27]~87_combout\,
	cout => \sec_count|cnt[27]~88\);

-- Location: FF_X4_Y1_N23
\sec_count|cnt[27]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[27]~87_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(27));

-- Location: LCCOMB_X3_Y1_N12
\sec_count|Equal0~8\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|Equal0~8_combout\ = (\sec_count|cnt\(26)) # ((\sec_count|cnt\(25)) # ((\sec_count|cnt\(27)) # (!\sec_count|cnt\(24))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111011111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(26),
	datab => \sec_count|cnt\(25),
	datac => \sec_count|cnt\(27),
	datad => \sec_count|cnt\(24),
	combout => \sec_count|Equal0~8_combout\);

-- Location: LCCOMB_X5_Y1_N14
\sec_count|Equal0~5\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|Equal0~5_combout\ = (!\sec_count|cnt\(19)) # (!\sec_count|cnt\(18))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000111111111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datac => \sec_count|cnt\(18),
	datad => \sec_count|cnt\(19),
	combout => \sec_count|Equal0~5_combout\);

-- Location: LCCOMB_X5_Y1_N2
\sec_count|Equal0~6\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|Equal0~6_combout\ = (\sec_count|cnt\(23)) # (((!\sec_count|cnt\(20)) # (!\sec_count|cnt\(22))) # (!\sec_count|cnt\(21)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1011111111111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(23),
	datab => \sec_count|cnt\(21),
	datac => \sec_count|cnt\(22),
	datad => \sec_count|cnt\(20),
	combout => \sec_count|Equal0~6_combout\);

-- Location: LCCOMB_X5_Y1_N16
\sec_count|Equal0~7\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|Equal0~7_combout\ = ((\sec_count|cnt\(17)) # ((\sec_count|Equal0~5_combout\) # (\sec_count|Equal0~6_combout\))) # (!\sec_count|cnt\(16))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111111111101",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(16),
	datab => \sec_count|cnt\(17),
	datac => \sec_count|Equal0~5_combout\,
	datad => \sec_count|Equal0~6_combout\,
	combout => \sec_count|Equal0~7_combout\);

-- Location: LCCOMB_X4_Y1_N24
\sec_count|cnt[28]~89\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[28]~89_combout\ = (\sec_count|cnt\(28) & (\sec_count|cnt[27]~88\ $ (GND))) # (!\sec_count|cnt\(28) & (!\sec_count|cnt[27]~88\ & VCC))
-- \sec_count|cnt[28]~90\ = CARRY((\sec_count|cnt\(28) & !\sec_count|cnt[27]~88\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(28),
	datad => VCC,
	cin => \sec_count|cnt[27]~88\,
	combout => \sec_count|cnt[28]~89_combout\,
	cout => \sec_count|cnt[28]~90\);

-- Location: FF_X4_Y1_N25
\sec_count|cnt[28]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[28]~89_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(28));

-- Location: LCCOMB_X4_Y1_N26
\sec_count|cnt[29]~91\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[29]~91_combout\ = (\sec_count|cnt\(29) & (!\sec_count|cnt[28]~90\)) # (!\sec_count|cnt\(29) & ((\sec_count|cnt[28]~90\) # (GND)))
-- \sec_count|cnt[29]~92\ = CARRY((!\sec_count|cnt[28]~90\) # (!\sec_count|cnt\(29)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101001011111",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(29),
	datad => VCC,
	cin => \sec_count|cnt[28]~90\,
	combout => \sec_count|cnt[29]~91_combout\,
	cout => \sec_count|cnt[29]~92\);

-- Location: FF_X4_Y1_N27
\sec_count|cnt[29]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[29]~91_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(29));

-- Location: LCCOMB_X4_Y1_N28
\sec_count|cnt[30]~93\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[30]~93_combout\ = (\sec_count|cnt\(30) & (\sec_count|cnt[29]~92\ $ (GND))) # (!\sec_count|cnt\(30) & (!\sec_count|cnt[29]~92\ & VCC))
-- \sec_count|cnt[30]~94\ = CARRY((\sec_count|cnt\(30) & !\sec_count|cnt[29]~92\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100001100001100",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|cnt\(30),
	datad => VCC,
	cin => \sec_count|cnt[29]~92\,
	combout => \sec_count|cnt[30]~93_combout\,
	cout => \sec_count|cnt[30]~94\);

-- Location: FF_X4_Y1_N29
\sec_count|cnt[30]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[30]~93_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(30));

-- Location: LCCOMB_X4_Y1_N30
\sec_count|cnt[31]~95\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|cnt[31]~95_combout\ = \sec_count|cnt\(31) $ (\sec_count|cnt[30]~94\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101001011010",
	sum_lutc_input => "cin")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(31),
	cin => \sec_count|cnt[30]~94\,
	combout => \sec_count|cnt[31]~95_combout\);

-- Location: FF_X4_Y1_N31
\sec_count|cnt[31]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|cnt[31]~95_combout\,
	sclr => \sec_count|cnt[4]~34_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|cnt\(31));

-- Location: LCCOMB_X5_Y1_N12
\sec_count|Equal0~9\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|Equal0~9_combout\ = (\sec_count|cnt\(29)) # ((\sec_count|cnt\(30)) # ((\sec_count|cnt\(28)) # (\sec_count|cnt\(31))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111111111110",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(29),
	datab => \sec_count|cnt\(30),
	datac => \sec_count|cnt\(28),
	datad => \sec_count|cnt\(31),
	combout => \sec_count|Equal0~9_combout\);

-- Location: LCCOMB_X5_Y1_N10
\sec_count|Equal0~3\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|Equal0~3_combout\ = (((\sec_count|cnt\(15)) # (!\sec_count|cnt\(14))) # (!\sec_count|cnt\(12))) # (!\sec_count|cnt\(13))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111011111111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(13),
	datab => \sec_count|cnt\(12),
	datac => \sec_count|cnt\(15),
	datad => \sec_count|cnt\(14),
	combout => \sec_count|Equal0~3_combout\);

-- Location: LCCOMB_X5_Y1_N28
\sec_count|Equal0~2\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|Equal0~2_combout\ = (\sec_count|cnt\(9)) # ((\sec_count|cnt\(8)) # ((\sec_count|cnt\(10)) # (!\sec_count|cnt\(11))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111011111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(9),
	datab => \sec_count|cnt\(8),
	datac => \sec_count|cnt\(10),
	datad => \sec_count|cnt\(11),
	combout => \sec_count|Equal0~2_combout\);

-- Location: LCCOMB_X5_Y1_N30
\sec_count|Equal0~1\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|Equal0~1_combout\ = (\sec_count|cnt\(6)) # (((\sec_count|cnt\(7)) # (!\sec_count|cnt\(5))) # (!\sec_count|cnt\(4)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111110111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(6),
	datab => \sec_count|cnt\(4),
	datac => \sec_count|cnt\(5),
	datad => \sec_count|cnt\(7),
	combout => \sec_count|Equal0~1_combout\);

-- Location: LCCOMB_X5_Y1_N20
\sec_count|Equal0~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|Equal0~0_combout\ = (((!\sec_count|cnt\(0)) # (!\sec_count|cnt\(2))) # (!\sec_count|cnt\(3))) # (!\sec_count|cnt\(1))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0111111111111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|cnt\(1),
	datab => \sec_count|cnt\(3),
	datac => \sec_count|cnt\(2),
	datad => \sec_count|cnt\(0),
	combout => \sec_count|Equal0~0_combout\);

-- Location: LCCOMB_X5_Y1_N24
\sec_count|Equal0~4\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|Equal0~4_combout\ = (\sec_count|Equal0~3_combout\) # ((\sec_count|Equal0~2_combout\) # ((\sec_count|Equal0~1_combout\) # (\sec_count|Equal0~0_combout\)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111111111110",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|Equal0~3_combout\,
	datab => \sec_count|Equal0~2_combout\,
	datac => \sec_count|Equal0~1_combout\,
	datad => \sec_count|Equal0~0_combout\,
	combout => \sec_count|Equal0~4_combout\);

-- Location: LCCOMB_X5_Y1_N18
\sec_count|Equal0~10\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|Equal0~10_combout\ = (\sec_count|Equal0~8_combout\) # ((\sec_count|Equal0~7_combout\) # ((\sec_count|Equal0~9_combout\) # (\sec_count|Equal0~4_combout\)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111111111110",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec_count|Equal0~8_combout\,
	datab => \sec_count|Equal0~7_combout\,
	datac => \sec_count|Equal0~9_combout\,
	datad => \sec_count|Equal0~4_combout\,
	combout => \sec_count|Equal0~10_combout\);

-- Location: LCCOMB_X5_Y1_N22
\sec_count|clk_out~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|clk_out~0_combout\ = \sec_count|clk_out~q\ $ (((!\reset~input_o\ & !\sec_count|Equal0~10_combout\)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100110011000011",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \sec_count|clk_out~q\,
	datac => \reset~input_o\,
	datad => \sec_count|Equal0~10_combout\,
	combout => \sec_count|clk_out~0_combout\);

-- Location: LCCOMB_X5_Y1_N26
\sec_count|clk_out~feeder\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec_count|clk_out~feeder_combout\ = \sec_count|clk_out~0_combout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \sec_count|clk_out~0_combout\,
	combout => \sec_count|clk_out~feeder_combout\);

-- Location: FF_X5_Y1_N27
\sec_count|clk_out\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \clk~inputclkctrl_outclk\,
	d => \sec_count|clk_out~feeder_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec_count|clk_out~q\);

-- Location: CLKCTRL_G1
\sec_count|clk_out~clkctrl\ : cycloneiv_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \sec_count|clk_out~clkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \sec_count|clk_out~clkctrl_outclk\);

-- Location: LCCOMB_X13_Y12_N24
\sec2min|qout[0]~7\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|qout[0]~7_combout\ = !\sec2min|qout\(0)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000111100001111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datac => \sec2min|qout\(0),
	combout => \sec2min|qout[0]~7_combout\);

-- Location: CLKCTRL_G19
\reset~inputclkctrl\ : cycloneiv_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \reset~inputclkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \reset~inputclkctrl_outclk\);

-- Location: FF_X13_Y12_N25
\sec2min|qout[0]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec_count|clk_out~clkctrl_outclk\,
	d => \sec2min|qout[0]~7_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec2min|qout\(0));

-- Location: LCCOMB_X13_Y12_N2
\sec2min|qout~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|qout~0_combout\ = (\sec2min|qout\(1) & (((!\sec2min|qout\(0))))) # (!\sec2min|qout\(1) & (\sec2min|qout\(0) & ((\sec2min|qout\(2)) # (!\sec2min|qout\(3)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101000110101010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(1),
	datab => \sec2min|qout\(3),
	datac => \sec2min|qout\(2),
	datad => \sec2min|qout\(0),
	combout => \sec2min|qout~0_combout\);

-- Location: FF_X13_Y12_N1
\sec2min|qout[1]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec_count|clk_out~clkctrl_outclk\,
	asdata => \sec2min|qout~0_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	sload => VCC,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec2min|qout\(1));

-- Location: LCCOMB_X12_Y12_N24
\sec2min|qout[2]~1\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|qout[2]~1_combout\ = \sec2min|qout\(2) $ (((\sec2min|qout\(0) & \sec2min|qout\(1))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011110011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \sec2min|qout\(0),
	datac => \sec2min|qout\(2),
	datad => \sec2min|qout\(1),
	combout => \sec2min|qout[2]~1_combout\);

-- Location: FF_X12_Y12_N25
\sec2min|qout[2]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec_count|clk_out~q\,
	d => \sec2min|qout[2]~1_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec2min|qout\(2));

-- Location: LCCOMB_X12_Y12_N12
\sec2min|qout~2\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|qout~2_combout\ = (\sec2min|qout\(2) & (\sec2min|qout\(3) $ (((\sec2min|qout\(0) & \sec2min|qout\(1)))))) # (!\sec2min|qout\(2) & (\sec2min|qout\(3) & ((\sec2min|qout\(1)) # (!\sec2min|qout\(0)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0111100010110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(2),
	datab => \sec2min|qout\(0),
	datac => \sec2min|qout\(3),
	datad => \sec2min|qout\(1),
	combout => \sec2min|qout~2_combout\);

-- Location: FF_X12_Y12_N13
\sec2min|qout[3]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec_count|clk_out~q\,
	d => \sec2min|qout~2_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec2min|qout\(3));

-- Location: LCCOMB_X13_Y12_N0
\show_sec_l|Mux7~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|Mux7~0_combout\ = ((!\sec2min|qout\(1) & !\sec2min|qout\(2))) # (!\sec2min|qout\(3))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101010101011111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(3),
	datac => \sec2min|qout\(1),
	datad => \sec2min|qout\(2),
	combout => \show_sec_l|Mux7~0_combout\);

-- Location: CLKCTRL_G0
\show_sec_l|Mux7~0clkctrl\ : cycloneiv_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \show_sec_l|Mux7~0clkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \show_sec_l|Mux7~0clkctrl_outclk\);

-- Location: LCCOMB_X13_Y12_N8
\show_sec_l|Mux0~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|Mux0~0_combout\ = (!\sec2min|qout\(1) & (!\sec2min|qout\(3) & (\sec2min|qout\(2) $ (\sec2min|qout\(0)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000100000010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(2),
	datab => \sec2min|qout\(1),
	datac => \sec2min|qout\(3),
	datad => \sec2min|qout\(0),
	combout => \show_sec_l|Mux0~0_combout\);

-- Location: LCCOMB_X13_Y12_N4
\show_sec_l|decodeout[0]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|decodeout\(0) = (GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & ((\show_sec_l|Mux0~0_combout\))) # (!GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & (\show_sec_l|decodeout\(0)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_sec_l|decodeout\(0),
	datac => \show_sec_l|Mux7~0clkctrl_outclk\,
	datad => \show_sec_l|Mux0~0_combout\,
	combout => \show_sec_l|decodeout\(0));

-- Location: LCCOMB_X13_Y12_N26
\show_sec_l|Mux1~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|Mux1~0_combout\ = (\sec2min|qout\(2) & (\sec2min|qout\(1) $ (\sec2min|qout\(0))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101000010100000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(1),
	datac => \sec2min|qout\(2),
	datad => \sec2min|qout\(0),
	combout => \show_sec_l|Mux1~0_combout\);

-- Location: LCCOMB_X13_Y12_N10
\show_sec_l|decodeout[1]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|decodeout\(1) = (GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & ((\show_sec_l|Mux1~0_combout\))) # (!GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & (\show_sec_l|decodeout\(1)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_sec_l|decodeout\(1),
	datac => \show_sec_l|Mux7~0clkctrl_outclk\,
	datad => \show_sec_l|Mux1~0_combout\,
	combout => \show_sec_l|decodeout\(1));

-- Location: LCCOMB_X13_Y12_N30
\show_sec_l|Mux2~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|Mux2~0_combout\ = (!\sec2min|qout\(2) & (\sec2min|qout\(1) & !\sec2min|qout\(0)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000001000100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(2),
	datab => \sec2min|qout\(1),
	datad => \sec2min|qout\(0),
	combout => \show_sec_l|Mux2~0_combout\);

-- Location: LCCOMB_X13_Y12_N18
\show_sec_l|decodeout[2]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|decodeout\(2) = (GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & (\show_sec_l|Mux2~0_combout\)) # (!GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & ((\show_sec_l|decodeout\(2))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1010111110100000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_sec_l|Mux2~0_combout\,
	datac => \show_sec_l|Mux7~0clkctrl_outclk\,
	datad => \show_sec_l|decodeout\(2),
	combout => \show_sec_l|decodeout\(2));

-- Location: LCCOMB_X13_Y12_N28
\show_sec_l|Mux3~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|Mux3~0_combout\ = (!\sec2min|qout\(3) & ((\sec2min|qout\(1) & (\sec2min|qout\(2) & \sec2min|qout\(0))) # (!\sec2min|qout\(1) & (\sec2min|qout\(2) $ (\sec2min|qout\(0))))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0100000100010000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(3),
	datab => \sec2min|qout\(1),
	datac => \sec2min|qout\(2),
	datad => \sec2min|qout\(0),
	combout => \show_sec_l|Mux3~0_combout\);

-- Location: LCCOMB_X13_Y12_N20
\show_sec_l|decodeout[3]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|decodeout\(3) = (GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & ((\show_sec_l|Mux3~0_combout\))) # (!GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & (\show_sec_l|decodeout\(3)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_sec_l|decodeout\(3),
	datac => \show_sec_l|Mux7~0clkctrl_outclk\,
	datad => \show_sec_l|Mux3~0_combout\,
	combout => \show_sec_l|decodeout\(3));

-- Location: LCCOMB_X13_Y12_N22
\show_sec_l|Mux4~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|Mux4~0_combout\ = (\sec2min|qout\(0)) # ((!\sec2min|qout\(1) & \sec2min|qout\(2)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \sec2min|qout\(1),
	datac => \sec2min|qout\(2),
	datad => \sec2min|qout\(0),
	combout => \show_sec_l|Mux4~0_combout\);

-- Location: LCCOMB_X13_Y12_N12
\show_sec_l|decodeout[4]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|decodeout\(4) = (GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & ((\show_sec_l|Mux4~0_combout\))) # (!GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & (\show_sec_l|decodeout\(4)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_sec_l|decodeout\(4),
	datac => \show_sec_l|Mux7~0clkctrl_outclk\,
	datad => \show_sec_l|Mux4~0_combout\,
	combout => \show_sec_l|decodeout\(4));

-- Location: LCCOMB_X13_Y12_N14
\show_sec_l|Mux5~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|Mux5~0_combout\ = (!\sec2min|qout\(3) & ((\sec2min|qout\(1) & ((\sec2min|qout\(0)) # (!\sec2min|qout\(2)))) # (!\sec2min|qout\(1) & (!\sec2min|qout\(2) & \sec2min|qout\(0)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0100010100000100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(3),
	datab => \sec2min|qout\(1),
	datac => \sec2min|qout\(2),
	datad => \sec2min|qout\(0),
	combout => \show_sec_l|Mux5~0_combout\);

-- Location: LCCOMB_X13_Y12_N6
\show_sec_l|decodeout[5]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|decodeout\(5) = (GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & ((\show_sec_l|Mux5~0_combout\))) # (!GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & (\show_sec_l|decodeout\(5)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_sec_l|decodeout\(5),
	datac => \show_sec_l|Mux7~0clkctrl_outclk\,
	datad => \show_sec_l|Mux5~0_combout\,
	combout => \show_sec_l|decodeout\(5));

-- Location: LCCOMB_X12_Y12_N18
\show_sec_l|Mux6~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|Mux6~0_combout\ = (\sec2min|qout\(3)) # ((\sec2min|qout\(2) & ((!\sec2min|qout\(0)) # (!\sec2min|qout\(1)))) # (!\sec2min|qout\(2) & (\sec2min|qout\(1))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1101111011111110",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(2),
	datab => \sec2min|qout\(3),
	datac => \sec2min|qout\(1),
	datad => \sec2min|qout\(0),
	combout => \show_sec_l|Mux6~0_combout\);

-- Location: LCCOMB_X12_Y12_N26
\show_sec_l|decodeout[6]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_l|decodeout\(6) = (GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & ((!\show_sec_l|Mux6~0_combout\))) # (!GLOBAL(\show_sec_l|Mux7~0clkctrl_outclk\) & (\show_sec_l|decodeout\(6)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000101011111010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_sec_l|decodeout\(6),
	datac => \show_sec_l|Mux7~0clkctrl_outclk\,
	datad => \show_sec_l|Mux6~0_combout\,
	combout => \show_sec_l|decodeout\(6));

-- Location: LCCOMB_X13_Y12_N16
\sec2min|Equal0~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|Equal0~0_combout\ = (\sec2min|qout\(3) & (!\sec2min|qout\(1) & (!\sec2min|qout\(2) & \sec2min|qout\(0))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000001000000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(3),
	datab => \sec2min|qout\(1),
	datac => \sec2min|qout\(2),
	datad => \sec2min|qout\(0),
	combout => \sec2min|Equal0~0_combout\);

-- Location: LCCOMB_X13_Y9_N16
\sec2min|qout[4]~8\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|qout[4]~8_combout\ = !\sec2min|qout\(4)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000011111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \sec2min|qout\(4),
	combout => \sec2min|qout[4]~8_combout\);

-- Location: FF_X14_Y9_N25
\sec2min|qout[4]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec_count|clk_out~clkctrl_outclk\,
	asdata => \sec2min|qout[4]~8_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	sload => VCC,
	ena => \sec2min|Equal0~0_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec2min|qout\(4));

-- Location: LCCOMB_X13_Y9_N0
\sec2min|qout~3\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|qout~3_combout\ = (\sec2min|qout\(5) & (((!\sec2min|qout\(4))))) # (!\sec2min|qout\(5) & (\sec2min|qout\(4) & ((\sec2min|qout\(7)) # (!\sec2min|qout\(6)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000110111110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(6),
	datab => \sec2min|qout\(7),
	datac => \sec2min|qout\(5),
	datad => \sec2min|qout\(4),
	combout => \sec2min|qout~3_combout\);

-- Location: FF_X14_Y9_N21
\sec2min|qout[5]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec_count|clk_out~clkctrl_outclk\,
	asdata => \sec2min|qout~3_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	sload => VCC,
	ena => \sec2min|Equal0~0_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec2min|qout\(5));

-- Location: LCCOMB_X14_Y9_N16
\sec2min|qout[7]~5\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|qout[7]~5_combout\ = (\sec2min|qout\(5) & (\sec2min|qout\(6) & \sec2min|qout\(4)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1000100000000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(5),
	datab => \sec2min|qout\(6),
	datad => \sec2min|qout\(4),
	combout => \sec2min|qout[7]~5_combout\);

-- Location: LCCOMB_X14_Y9_N12
\sec2min|qout[7]~6\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|qout[7]~6_combout\ = \sec2min|qout\(7) $ (((\sec2min|Equal0~0_combout\ & \sec2min|qout[7]~5_combout\)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|Equal0~0_combout\,
	datac => \sec2min|qout\(7),
	datad => \sec2min|qout[7]~5_combout\,
	combout => \sec2min|qout[7]~6_combout\);

-- Location: FF_X14_Y9_N13
\sec2min|qout[7]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec_count|clk_out~q\,
	d => \sec2min|qout[7]~6_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec2min|qout\(7));

-- Location: LCCOMB_X13_Y9_N28
\sec2min|qout~4\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|qout~4_combout\ = (\sec2min|qout\(5) & ((\sec2min|qout\(6) $ (\sec2min|qout\(4))))) # (!\sec2min|qout\(5) & (\sec2min|qout\(6) & ((\sec2min|qout\(7)) # (!\sec2min|qout\(4)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0010110011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(7),
	datab => \sec2min|qout\(5),
	datac => \sec2min|qout\(6),
	datad => \sec2min|qout\(4),
	combout => \sec2min|qout~4_combout\);

-- Location: FF_X13_Y9_N29
\sec2min|qout[6]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec_count|clk_out~q\,
	d => \sec2min|qout~4_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	ena => \sec2min|Equal0~0_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec2min|qout\(6));

-- Location: LCCOMB_X14_Y9_N20
\show_sec_h|Mux7~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|Mux7~0_combout\ = ((!\sec2min|qout\(6) & !\sec2min|qout\(5))) # (!\sec2min|qout\(7))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000001111111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \sec2min|qout\(6),
	datac => \sec2min|qout\(5),
	datad => \sec2min|qout\(7),
	combout => \show_sec_h|Mux7~0_combout\);

-- Location: CLKCTRL_G18
\show_sec_h|Mux7~0clkctrl\ : cycloneiv_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \show_sec_h|Mux7~0clkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \show_sec_h|Mux7~0clkctrl_outclk\);

-- Location: LCCOMB_X13_Y9_N12
\show_sec_h|Mux0~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|Mux0~0_combout\ = (!\sec2min|qout\(7) & (!\sec2min|qout\(5) & (\sec2min|qout\(6) $ (\sec2min|qout\(4)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000100000100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(7),
	datab => \sec2min|qout\(6),
	datac => \sec2min|qout\(5),
	datad => \sec2min|qout\(4),
	combout => \show_sec_h|Mux0~0_combout\);

-- Location: LCCOMB_X13_Y9_N6
\show_sec_h|decodeout[0]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|decodeout\(0) = (GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & ((\show_sec_h|Mux0~0_combout\))) # (!GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & (\show_sec_h|decodeout\(0)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_sec_h|decodeout\(0),
	datac => \show_sec_h|Mux7~0clkctrl_outclk\,
	datad => \show_sec_h|Mux0~0_combout\,
	combout => \show_sec_h|decodeout\(0));

-- Location: LCCOMB_X14_Y9_N4
\show_sec_h|Mux1~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|Mux1~0_combout\ = (\sec2min|qout\(6) & (\sec2min|qout\(5) $ (\sec2min|qout\(4))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000110011000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \sec2min|qout\(6),
	datac => \sec2min|qout\(5),
	datad => \sec2min|qout\(4),
	combout => \show_sec_h|Mux1~0_combout\);

-- Location: LCCOMB_X14_Y9_N10
\show_sec_h|decodeout[1]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|decodeout\(1) = (GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & ((\show_sec_h|Mux1~0_combout\))) # (!GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & (\show_sec_h|decodeout\(1)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_sec_h|decodeout\(1),
	datac => \show_sec_h|Mux7~0clkctrl_outclk\,
	datad => \show_sec_h|Mux1~0_combout\,
	combout => \show_sec_h|decodeout\(1));

-- Location: LCCOMB_X14_Y9_N14
\show_sec_h|Mux2~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|Mux2~0_combout\ = (!\sec2min|qout\(6) & (\sec2min|qout\(5) & !\sec2min|qout\(4)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000000110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \sec2min|qout\(6),
	datac => \sec2min|qout\(5),
	datad => \sec2min|qout\(4),
	combout => \show_sec_h|Mux2~0_combout\);

-- Location: LCCOMB_X14_Y9_N0
\show_sec_h|decodeout[2]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|decodeout\(2) = (GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & ((\show_sec_h|Mux2~0_combout\))) # (!GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & (\show_sec_h|decodeout\(2)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_sec_h|decodeout\(2),
	datac => \show_sec_h|Mux7~0clkctrl_outclk\,
	datad => \show_sec_h|Mux2~0_combout\,
	combout => \show_sec_h|decodeout\(2));

-- Location: LCCOMB_X14_Y9_N30
\show_sec_h|Mux3~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|Mux3~0_combout\ = (!\sec2min|qout\(7) & ((\sec2min|qout\(6) & (\sec2min|qout\(5) $ (!\sec2min|qout\(4)))) # (!\sec2min|qout\(6) & (!\sec2min|qout\(5) & \sec2min|qout\(4)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0100000100000100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(7),
	datab => \sec2min|qout\(6),
	datac => \sec2min|qout\(5),
	datad => \sec2min|qout\(4),
	combout => \show_sec_h|Mux3~0_combout\);

-- Location: LCCOMB_X14_Y9_N2
\show_sec_h|decodeout[3]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|decodeout\(3) = (GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & (\show_sec_h|Mux3~0_combout\)) # (!GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & ((\show_sec_h|decodeout\(3))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1010111110100000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_sec_h|Mux3~0_combout\,
	datac => \show_sec_h|Mux7~0clkctrl_outclk\,
	datad => \show_sec_h|decodeout\(3),
	combout => \show_sec_h|decodeout\(3));

-- Location: LCCOMB_X14_Y9_N8
\show_sec_h|Mux4~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|Mux4~0_combout\ = (\sec2min|qout\(4)) # ((\sec2min|qout\(6) & !\sec2min|qout\(5)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \sec2min|qout\(6),
	datac => \sec2min|qout\(5),
	datad => \sec2min|qout\(4),
	combout => \show_sec_h|Mux4~0_combout\);

-- Location: LCCOMB_X14_Y9_N18
\show_sec_h|decodeout[4]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|decodeout\(4) = (GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & ((\show_sec_h|Mux4~0_combout\))) # (!GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & (\show_sec_h|decodeout\(4)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_sec_h|decodeout\(4),
	datac => \show_sec_h|Mux7~0clkctrl_outclk\,
	datad => \show_sec_h|Mux4~0_combout\,
	combout => \show_sec_h|decodeout\(4));

-- Location: LCCOMB_X14_Y9_N22
\show_sec_h|Mux5~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|Mux5~0_combout\ = (!\sec2min|qout\(7) & ((\sec2min|qout\(6) & (\sec2min|qout\(5) & \sec2min|qout\(4))) # (!\sec2min|qout\(6) & ((\sec2min|qout\(5)) # (\sec2min|qout\(4))))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101000100010000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(7),
	datab => \sec2min|qout\(6),
	datac => \sec2min|qout\(5),
	datad => \sec2min|qout\(4),
	combout => \show_sec_h|Mux5~0_combout\);

-- Location: LCCOMB_X14_Y9_N28
\show_sec_h|decodeout[5]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|decodeout\(5) = (GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & ((\show_sec_h|Mux5~0_combout\))) # (!GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & (\show_sec_h|decodeout\(5)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_sec_h|decodeout\(5),
	datac => \show_sec_h|Mux7~0clkctrl_outclk\,
	datad => \show_sec_h|Mux5~0_combout\,
	combout => \show_sec_h|decodeout\(5));

-- Location: LCCOMB_X14_Y9_N26
\show_sec_h|Mux6~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|Mux6~0_combout\ = (\sec2min|qout\(7)) # ((\sec2min|qout\(6) & ((!\sec2min|qout\(4)) # (!\sec2min|qout\(5)))) # (!\sec2min|qout\(6) & (\sec2min|qout\(5))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1011111011111110",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(7),
	datab => \sec2min|qout\(6),
	datac => \sec2min|qout\(5),
	datad => \sec2min|qout\(4),
	combout => \show_sec_h|Mux6~0_combout\);

-- Location: LCCOMB_X14_Y9_N6
\show_sec_h|decodeout[6]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_sec_h|decodeout\(6) = (GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & ((!\show_sec_h|Mux6~0_combout\))) # (!GLOBAL(\show_sec_h|Mux7~0clkctrl_outclk\) & (\show_sec_h|decodeout\(6)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000101011111010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_sec_h|decodeout\(6),
	datac => \show_sec_h|Mux7~0clkctrl_outclk\,
	datad => \show_sec_h|Mux6~0_combout\,
	combout => \show_sec_h|decodeout\(6));

-- Location: LCCOMB_X14_Y9_N24
\sec2min|Equal1~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|Equal1~0_combout\ = (!\sec2min|qout\(7) & (\sec2min|qout\(6) & (\sec2min|qout\(4) & !\sec2min|qout\(5))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000001000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|qout\(7),
	datab => \sec2min|qout\(6),
	datac => \sec2min|qout\(4),
	datad => \sec2min|qout\(5),
	combout => \sec2min|Equal1~0_combout\);

-- Location: LCCOMB_X15_Y13_N28
\sec2min|Equal2~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \sec2min|Equal2~0_combout\ = (\sec2min|Equal0~0_combout\ & \sec2min|Equal1~0_combout\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1010000010100000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \sec2min|Equal0~0_combout\,
	datac => \sec2min|Equal1~0_combout\,
	combout => \sec2min|Equal2~0_combout\);

-- Location: FF_X15_Y13_N29
\sec2min|cout\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec_count|clk_out~clkctrl_outclk\,
	d => \sec2min|Equal2~0_combout\,
	ena => \ALT_INV_reset~input_o\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \sec2min|cout~q\);

-- Location: CLKCTRL_G3
\sec2min|cout~clkctrl\ : cycloneiv_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \sec2min|cout~clkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \sec2min|cout~clkctrl_outclk\);

-- Location: LCCOMB_X15_Y13_N16
\min2hour|qout[0]~7\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|qout[0]~7_combout\ = !\min2hour|qout\(0)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000011111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \min2hour|qout\(0),
	combout => \min2hour|qout[0]~7_combout\);

-- Location: FF_X16_Y13_N3
\min2hour|qout[0]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec2min|cout~clkctrl_outclk\,
	asdata => \min2hour|qout[0]~7_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	sload => VCC,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \min2hour|qout\(0));

-- Location: LCCOMB_X16_Y13_N12
\min2hour|qout~2\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|qout~2_combout\ = (\min2hour|qout\(2) & (\min2hour|qout\(3) $ (((\min2hour|qout\(1) & \min2hour|qout\(0)))))) # (!\min2hour|qout\(2) & (\min2hour|qout\(3) & ((\min2hour|qout\(1)) # (!\min2hour|qout\(0)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0110100011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(2),
	datab => \min2hour|qout\(1),
	datac => \min2hour|qout\(3),
	datad => \min2hour|qout\(0),
	combout => \min2hour|qout~2_combout\);

-- Location: FF_X16_Y13_N13
\min2hour|qout[3]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec2min|cout~q\,
	d => \min2hour|qout~2_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \min2hour|qout\(3));

-- Location: LCCOMB_X16_Y13_N2
\min2hour|qout~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|qout~0_combout\ = (\min2hour|qout\(1) & (((!\min2hour|qout\(0))))) # (!\min2hour|qout\(1) & (\min2hour|qout\(0) & ((\min2hour|qout\(2)) # (!\min2hour|qout\(3)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0010110000111100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(2),
	datab => \min2hour|qout\(1),
	datac => \min2hour|qout\(0),
	datad => \min2hour|qout\(3),
	combout => \min2hour|qout~0_combout\);

-- Location: FF_X16_Y13_N25
\min2hour|qout[1]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec2min|cout~clkctrl_outclk\,
	asdata => \min2hour|qout~0_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	sload => VCC,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \min2hour|qout\(1));

-- Location: LCCOMB_X15_Y13_N14
\min2hour|qout[2]~1\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|qout[2]~1_combout\ = \min2hour|qout\(2) $ (((\min2hour|qout\(1) & \min2hour|qout\(0))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011110011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \min2hour|qout\(1),
	datac => \min2hour|qout\(2),
	datad => \min2hour|qout\(0),
	combout => \min2hour|qout[2]~1_combout\);

-- Location: FF_X15_Y13_N15
\min2hour|qout[2]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec2min|cout~q\,
	d => \min2hour|qout[2]~1_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \min2hour|qout\(2));

-- Location: LCCOMB_X16_Y13_N24
\show_min_l|Mux7~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|Mux7~0_combout\ = ((!\min2hour|qout\(2) & !\min2hour|qout\(1))) # (!\min2hour|qout\(3))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000010111111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(2),
	datac => \min2hour|qout\(1),
	datad => \min2hour|qout\(3),
	combout => \show_min_l|Mux7~0_combout\);

-- Location: CLKCTRL_G15
\show_min_l|Mux7~0clkctrl\ : cycloneiv_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \show_min_l|Mux7~0clkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \show_min_l|Mux7~0clkctrl_outclk\);

-- Location: LCCOMB_X16_Y13_N8
\show_min_l|Mux0~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|Mux0~0_combout\ = (!\min2hour|qout\(3) & (!\min2hour|qout\(1) & (\min2hour|qout\(2) $ (\min2hour|qout\(0)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000100000100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(3),
	datab => \min2hour|qout\(2),
	datac => \min2hour|qout\(1),
	datad => \min2hour|qout\(0),
	combout => \show_min_l|Mux0~0_combout\);

-- Location: LCCOMB_X16_Y13_N6
\show_min_l|decodeout[0]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|decodeout\(0) = (GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & ((\show_min_l|Mux0~0_combout\))) # (!GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & (\show_min_l|decodeout\(0)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_min_l|decodeout\(0),
	datac => \show_min_l|Mux7~0clkctrl_outclk\,
	datad => \show_min_l|Mux0~0_combout\,
	combout => \show_min_l|decodeout\(0));

-- Location: LCCOMB_X16_Y13_N26
\show_min_l|Mux1~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|Mux1~0_combout\ = (\min2hour|qout\(2) & (\min2hour|qout\(1) $ (\min2hour|qout\(0))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000110011000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \min2hour|qout\(2),
	datac => \min2hour|qout\(1),
	datad => \min2hour|qout\(0),
	combout => \show_min_l|Mux1~0_combout\);

-- Location: LCCOMB_X16_Y13_N0
\show_min_l|decodeout[1]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|decodeout\(1) = (GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & ((\show_min_l|Mux1~0_combout\))) # (!GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & (\show_min_l|decodeout\(1)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_min_l|decodeout\(1),
	datac => \show_min_l|Mux7~0clkctrl_outclk\,
	datad => \show_min_l|Mux1~0_combout\,
	combout => \show_min_l|decodeout\(1));

-- Location: LCCOMB_X16_Y13_N28
\show_min_l|Mux2~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|Mux2~0_combout\ = (!\min2hour|qout\(2) & (\min2hour|qout\(1) & !\min2hour|qout\(0)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000000110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \min2hour|qout\(2),
	datac => \min2hour|qout\(1),
	datad => \min2hour|qout\(0),
	combout => \show_min_l|Mux2~0_combout\);

-- Location: LCCOMB_X16_Y13_N18
\show_min_l|decodeout[2]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|decodeout\(2) = (GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & ((\show_min_l|Mux2~0_combout\))) # (!GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & (\show_min_l|decodeout\(2)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_min_l|decodeout\(2),
	datac => \show_min_l|Mux7~0clkctrl_outclk\,
	datad => \show_min_l|Mux2~0_combout\,
	combout => \show_min_l|decodeout\(2));

-- Location: LCCOMB_X16_Y13_N14
\show_min_l|Mux3~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|Mux3~0_combout\ = (!\min2hour|qout\(3) & ((\min2hour|qout\(2) & (\min2hour|qout\(1) $ (!\min2hour|qout\(0)))) # (!\min2hour|qout\(2) & (!\min2hour|qout\(1) & \min2hour|qout\(0)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0100000100000100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(3),
	datab => \min2hour|qout\(2),
	datac => \min2hour|qout\(1),
	datad => \min2hour|qout\(0),
	combout => \show_min_l|Mux3~0_combout\);

-- Location: LCCOMB_X16_Y13_N20
\show_min_l|decodeout[3]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|decodeout\(3) = (GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & ((\show_min_l|Mux3~0_combout\))) # (!GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & (\show_min_l|decodeout\(3)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_min_l|decodeout\(3),
	datac => \show_min_l|Mux7~0clkctrl_outclk\,
	datad => \show_min_l|Mux3~0_combout\,
	combout => \show_min_l|decodeout\(3));

-- Location: LCCOMB_X16_Y13_N22
\show_min_l|Mux4~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|Mux4~0_combout\ = (\min2hour|qout\(0)) # ((\min2hour|qout\(2) & !\min2hour|qout\(1)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \min2hour|qout\(2),
	datac => \min2hour|qout\(1),
	datad => \min2hour|qout\(0),
	combout => \show_min_l|Mux4~0_combout\);

-- Location: LCCOMB_X16_Y13_N10
\show_min_l|decodeout[4]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|decodeout\(4) = (GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & ((\show_min_l|Mux4~0_combout\))) # (!GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & (\show_min_l|decodeout\(4)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_min_l|decodeout\(4),
	datac => \show_min_l|Mux7~0clkctrl_outclk\,
	datad => \show_min_l|Mux4~0_combout\,
	combout => \show_min_l|decodeout\(4));

-- Location: LCCOMB_X15_Y13_N24
\show_min_l|Mux5~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|Mux5~0_combout\ = (!\min2hour|qout\(3) & ((\min2hour|qout\(2) & (\min2hour|qout\(1) & \min2hour|qout\(0))) # (!\min2hour|qout\(2) & ((\min2hour|qout\(1)) # (\min2hour|qout\(0))))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011000100010000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(2),
	datab => \min2hour|qout\(3),
	datac => \min2hour|qout\(1),
	datad => \min2hour|qout\(0),
	combout => \show_min_l|Mux5~0_combout\);

-- Location: LCCOMB_X15_Y13_N26
\show_min_l|decodeout[5]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|decodeout\(5) = (GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & ((\show_min_l|Mux5~0_combout\))) # (!GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & (\show_min_l|decodeout\(5)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_min_l|decodeout\(5),
	datac => \show_min_l|Mux7~0clkctrl_outclk\,
	datad => \show_min_l|Mux5~0_combout\,
	combout => \show_min_l|decodeout\(5));

-- Location: LCCOMB_X16_Y13_N4
\show_min_l|Mux6~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|Mux6~0_combout\ = (\min2hour|qout\(3)) # ((\min2hour|qout\(2) & ((!\min2hour|qout\(0)) # (!\min2hour|qout\(1)))) # (!\min2hour|qout\(2) & (\min2hour|qout\(1))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1011111011111110",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(3),
	datab => \min2hour|qout\(2),
	datac => \min2hour|qout\(1),
	datad => \min2hour|qout\(0),
	combout => \show_min_l|Mux6~0_combout\);

-- Location: LCCOMB_X16_Y13_N16
\show_min_l|decodeout[6]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_l|decodeout\(6) = (GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & ((!\show_min_l|Mux6~0_combout\))) # (!GLOBAL(\show_min_l|Mux7~0clkctrl_outclk\) & (\show_min_l|decodeout\(6)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000110011111100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_min_l|decodeout\(6),
	datac => \show_min_l|Mux7~0clkctrl_outclk\,
	datad => \show_min_l|Mux6~0_combout\,
	combout => \show_min_l|decodeout\(6));

-- Location: LCCOMB_X14_Y12_N16
\min2hour|qout[4]~8\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|qout[4]~8_combout\ = !\min2hour|qout\(4)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000111100001111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datac => \min2hour|qout\(4),
	combout => \min2hour|qout[4]~8_combout\);

-- Location: LCCOMB_X16_Y13_N30
\min2hour|Equal0~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|Equal0~0_combout\ = (!\min2hour|qout\(2) & (!\min2hour|qout\(1) & (\min2hour|qout\(3) & \min2hour|qout\(0))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0001000000000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(2),
	datab => \min2hour|qout\(1),
	datac => \min2hour|qout\(3),
	datad => \min2hour|qout\(0),
	combout => \min2hour|Equal0~0_combout\);

-- Location: FF_X15_Y12_N11
\min2hour|qout[4]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec2min|cout~clkctrl_outclk\,
	asdata => \min2hour|qout[4]~8_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	sload => VCC,
	ena => \min2hour|Equal0~0_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \min2hour|qout\(4));

-- Location: LCCOMB_X16_Y12_N10
\min2hour|qout~3\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|qout~3_combout\ = (\min2hour|qout\(4) & (!\min2hour|qout\(5) & ((\min2hour|qout\(7)) # (!\min2hour|qout\(6))))) # (!\min2hour|qout\(4) & (((\min2hour|qout\(5)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101010110100010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(4),
	datab => \min2hour|qout\(6),
	datac => \min2hour|qout\(7),
	datad => \min2hour|qout\(5),
	combout => \min2hour|qout~3_combout\);

-- Location: FF_X15_Y12_N15
\min2hour|qout[5]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec2min|cout~clkctrl_outclk\,
	asdata => \min2hour|qout~3_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	sload => VCC,
	ena => \min2hour|Equal0~0_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \min2hour|qout\(5));

-- Location: LCCOMB_X16_Y12_N20
\min2hour|qout[7]~5\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|qout[7]~5_combout\ = (\min2hour|qout\(6) & (\min2hour|qout\(5) & \min2hour|qout\(4)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100000000000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \min2hour|qout\(6),
	datac => \min2hour|qout\(5),
	datad => \min2hour|qout\(4),
	combout => \min2hour|qout[7]~5_combout\);

-- Location: LCCOMB_X16_Y12_N26
\min2hour|qout[7]~6\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|qout[7]~6_combout\ = \min2hour|qout\(7) $ (((\min2hour|Equal0~0_combout\ & \min2hour|qout[7]~5_combout\)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011110011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \min2hour|Equal0~0_combout\,
	datac => \min2hour|qout\(7),
	datad => \min2hour|qout[7]~5_combout\,
	combout => \min2hour|qout[7]~6_combout\);

-- Location: FF_X16_Y12_N27
\min2hour|qout[7]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec2min|cout~q\,
	d => \min2hour|qout[7]~6_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \min2hour|qout\(7));

-- Location: LCCOMB_X15_Y12_N6
\min2hour|qout~4\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|qout~4_combout\ = (\min2hour|qout\(4) & ((\min2hour|qout\(6) & (\min2hour|qout\(7) & !\min2hour|qout\(5))) # (!\min2hour|qout\(6) & ((\min2hour|qout\(5)))))) # (!\min2hour|qout\(4) & (((\min2hour|qout\(6)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101101011010000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(4),
	datab => \min2hour|qout\(7),
	datac => \min2hour|qout\(6),
	datad => \min2hour|qout\(5),
	combout => \min2hour|qout~4_combout\);

-- Location: FF_X15_Y12_N7
\min2hour|qout[6]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec2min|cout~q\,
	d => \min2hour|qout~4_combout\,
	clrn => \ALT_INV_reset~inputclkctrl_outclk\,
	ena => \min2hour|Equal0~0_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \min2hour|qout\(6));

-- Location: LCCOMB_X15_Y12_N14
\show_min_h|Mux7~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|Mux7~0_combout\ = ((!\min2hour|qout\(6) & !\min2hour|qout\(5))) # (!\min2hour|qout\(7))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000010111111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(6),
	datac => \min2hour|qout\(5),
	datad => \min2hour|qout\(7),
	combout => \show_min_h|Mux7~0_combout\);

-- Location: CLKCTRL_G2
\show_min_h|Mux7~0clkctrl\ : cycloneiv_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \show_min_h|Mux7~0clkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \show_min_h|Mux7~0clkctrl_outclk\);

-- Location: LCCOMB_X15_Y12_N16
\show_min_h|Mux0~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|Mux0~0_combout\ = (!\min2hour|qout\(5) & (!\min2hour|qout\(7) & (\min2hour|qout\(6) $ (\min2hour|qout\(4)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000100000100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(5),
	datab => \min2hour|qout\(6),
	datac => \min2hour|qout\(7),
	datad => \min2hour|qout\(4),
	combout => \show_min_h|Mux0~0_combout\);

-- Location: LCCOMB_X15_Y12_N10
\show_min_h|decodeout[0]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|decodeout\(0) = (GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & ((\show_min_h|Mux0~0_combout\))) # (!GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & (\show_min_h|decodeout\(0)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1110111001000100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_min_h|Mux7~0clkctrl_outclk\,
	datab => \show_min_h|decodeout\(0),
	datad => \show_min_h|Mux0~0_combout\,
	combout => \show_min_h|decodeout\(0));

-- Location: LCCOMB_X15_Y12_N0
\show_min_h|Mux1~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|Mux1~0_combout\ = (\min2hour|qout\(6) & (\min2hour|qout\(5) $ (\min2hour|qout\(4))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0100010010001000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(5),
	datab => \min2hour|qout\(6),
	datad => \min2hour|qout\(4),
	combout => \show_min_h|Mux1~0_combout\);

-- Location: LCCOMB_X15_Y12_N26
\show_min_h|decodeout[1]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|decodeout\(1) = (GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & ((\show_min_h|Mux1~0_combout\))) # (!GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & (\show_min_h|decodeout\(1)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_min_h|decodeout\(1),
	datac => \show_min_h|Mux7~0clkctrl_outclk\,
	datad => \show_min_h|Mux1~0_combout\,
	combout => \show_min_h|decodeout\(1));

-- Location: LCCOMB_X15_Y12_N20
\show_min_h|Mux2~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|Mux2~0_combout\ = (\min2hour|qout\(5) & (!\min2hour|qout\(6) & !\min2hour|qout\(4)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000000100010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(5),
	datab => \min2hour|qout\(6),
	datad => \min2hour|qout\(4),
	combout => \show_min_h|Mux2~0_combout\);

-- Location: LCCOMB_X15_Y12_N22
\show_min_h|decodeout[2]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|decodeout\(2) = (GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & ((\show_min_h|Mux2~0_combout\))) # (!GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & (\show_min_h|decodeout\(2)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_min_h|decodeout\(2),
	datac => \show_min_h|Mux7~0clkctrl_outclk\,
	datad => \show_min_h|Mux2~0_combout\,
	combout => \show_min_h|decodeout\(2));

-- Location: LCCOMB_X15_Y12_N2
\show_min_h|Mux3~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|Mux3~0_combout\ = (!\min2hour|qout\(7) & ((\min2hour|qout\(5) & (\min2hour|qout\(6) & \min2hour|qout\(4))) # (!\min2hour|qout\(5) & (\min2hour|qout\(6) $ (\min2hour|qout\(4))))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000100100000100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(5),
	datab => \min2hour|qout\(6),
	datac => \min2hour|qout\(7),
	datad => \min2hour|qout\(4),
	combout => \show_min_h|Mux3~0_combout\);

-- Location: LCCOMB_X15_Y12_N30
\show_min_h|decodeout[3]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|decodeout\(3) = (GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & ((\show_min_h|Mux3~0_combout\))) # (!GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & (\show_min_h|decodeout\(3)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_min_h|decodeout\(3),
	datac => \show_min_h|Mux7~0clkctrl_outclk\,
	datad => \show_min_h|Mux3~0_combout\,
	combout => \show_min_h|decodeout\(3));

-- Location: LCCOMB_X15_Y12_N18
\show_min_h|Mux4~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|Mux4~0_combout\ = (\min2hour|qout\(4)) # ((!\min2hour|qout\(5) & \min2hour|qout\(6)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111101000100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(5),
	datab => \min2hour|qout\(6),
	datad => \min2hour|qout\(4),
	combout => \show_min_h|Mux4~0_combout\);

-- Location: LCCOMB_X15_Y12_N4
\show_min_h|decodeout[4]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|decodeout\(4) = (GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & ((\show_min_h|Mux4~0_combout\))) # (!GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & (\show_min_h|decodeout\(4)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_min_h|decodeout\(4),
	datac => \show_min_h|Mux7~0clkctrl_outclk\,
	datad => \show_min_h|Mux4~0_combout\,
	combout => \show_min_h|decodeout\(4));

-- Location: LCCOMB_X15_Y12_N24
\show_min_h|Mux5~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|Mux5~0_combout\ = (!\min2hour|qout\(7) & ((\min2hour|qout\(5) & ((\min2hour|qout\(4)) # (!\min2hour|qout\(6)))) # (!\min2hour|qout\(5) & (!\min2hour|qout\(6) & \min2hour|qout\(4)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000101100000010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(5),
	datab => \min2hour|qout\(6),
	datac => \min2hour|qout\(7),
	datad => \min2hour|qout\(4),
	combout => \show_min_h|Mux5~0_combout\);

-- Location: LCCOMB_X15_Y12_N28
\show_min_h|decodeout[5]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|decodeout\(5) = (GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & ((\show_min_h|Mux5~0_combout\))) # (!GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & (\show_min_h|decodeout\(5)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_min_h|decodeout\(5),
	datac => \show_min_h|Mux7~0clkctrl_outclk\,
	datad => \show_min_h|Mux5~0_combout\,
	combout => \show_min_h|decodeout\(5));

-- Location: LCCOMB_X15_Y12_N12
\show_min_h|Mux6~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|Mux6~0_combout\ = (\min2hour|qout\(7)) # ((\min2hour|qout\(6) & ((!\min2hour|qout\(4)) # (!\min2hour|qout\(5)))) # (!\min2hour|qout\(6) & (\min2hour|qout\(5))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1011111011111110",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(7),
	datab => \min2hour|qout\(6),
	datac => \min2hour|qout\(5),
	datad => \min2hour|qout\(4),
	combout => \show_min_h|Mux6~0_combout\);

-- Location: LCCOMB_X15_Y12_N8
\show_min_h|decodeout[6]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_min_h|decodeout\(6) = (GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & ((!\show_min_h|Mux6~0_combout\))) # (!GLOBAL(\show_min_h|Mux7~0clkctrl_outclk\) & (\show_min_h|decodeout\(6)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000110011111100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_min_h|decodeout\(6),
	datac => \show_min_h|Mux7~0clkctrl_outclk\,
	datad => \show_min_h|Mux6~0_combout\,
	combout => \show_min_h|decodeout\(6));

-- Location: LCCOMB_X16_Y12_N12
\min2hour|Equal1~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|Equal1~0_combout\ = (!\min2hour|qout\(7) & (\min2hour|qout\(6) & (\min2hour|qout\(4) & !\min2hour|qout\(5))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000001000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \min2hour|qout\(7),
	datab => \min2hour|qout\(6),
	datac => \min2hour|qout\(4),
	datad => \min2hour|qout\(5),
	combout => \min2hour|Equal1~0_combout\);

-- Location: LCCOMB_X16_Y14_N0
\min2hour|Equal2~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \min2hour|Equal2~0_combout\ = (\min2hour|Equal0~0_combout\ & \min2hour|Equal1~0_combout\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1100000011000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \min2hour|Equal0~0_combout\,
	datac => \min2hour|Equal1~0_combout\,
	combout => \min2hour|Equal2~0_combout\);

-- Location: FF_X16_Y14_N1
\min2hour|cout\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \sec2min|cout~clkctrl_outclk\,
	d => \min2hour|Equal2~0_combout\,
	ena => \ALT_INV_reset~input_o\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => \min2hour|cout~q\);

-- Location: CLKCTRL_G4
\min2hour|cout~clkctrl\ : cycloneiv_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \min2hour|cout~clkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \min2hour|cout~clkctrl_outclk\);

-- Location: LCCOMB_X15_Y14_N10
\hour~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \hour~0_combout\ = (!hour(0) & !\reset~input_o\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000000001111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datac => hour(0),
	datad => \reset~input_o\,
	combout => \hour~0_combout\);

-- Location: FF_X15_Y14_N11
\hour[0]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \min2hour|ALT_INV_cout~clkctrl_outclk\,
	d => \hour~0_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => hour(0));

-- Location: LCCOMB_X16_Y14_N20
\hour~1\ : cycloneiv_lcell_comb
-- Equation(s):
-- \hour~1_combout\ = (!\reset~input_o\ & (hour(1) $ (hour(0))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000010101010000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \reset~input_o\,
	datac => hour(1),
	datad => hour(0),
	combout => \hour~1_combout\);

-- Location: LCCOMB_X16_Y14_N18
\hour[1]~feeder\ : cycloneiv_lcell_comb
-- Equation(s):
-- \hour[1]~feeder_combout\ = \hour~1_combout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111000011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datac => \hour~1_combout\,
	combout => \hour[1]~feeder_combout\);

-- Location: FF_X16_Y14_N19
\hour[1]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \min2hour|ALT_INV_cout~clkctrl_outclk\,
	d => \hour[1]~feeder_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => hour(1));

-- Location: LCCOMB_X15_Y14_N6
\Add0~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \Add0~0_combout\ = hour(2) $ (((hour(1) & hour(0))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011110011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => hour(1),
	datac => hour(2),
	datad => hour(0),
	combout => \Add0~0_combout\);

-- Location: FF_X15_Y14_N7
\hour[2]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \min2hour|ALT_INV_cout~q\,
	d => \Add0~0_combout\,
	sclr => \reset~input_o\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => hour(2));

-- Location: LCCOMB_X15_Y14_N18
\Add0~1\ : cycloneiv_lcell_comb
-- Equation(s):
-- \Add0~1_combout\ = hour(3) $ (((hour(2) & (hour(1) & hour(0)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0111100011110000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => hour(2),
	datab => hour(1),
	datac => hour(3),
	datad => hour(0),
	combout => \Add0~1_combout\);

-- Location: FF_X15_Y14_N19
\hour[3]\ : dffeas
-- pragma translate_off
GENERIC MAP (
	is_wysiwyg => "true",
	power_up => "low")
-- pragma translate_on
PORT MAP (
	clk => \min2hour|ALT_INV_cout~q\,
	d => \Add0~1_combout\,
	sclr => \reset~input_o\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	q => hour(3));

-- Location: LCCOMB_X16_Y14_N24
\show_hour|Mux7~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|Mux7~0_combout\ = ((!hour(1) & !hour(2))) # (!hour(3))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000001111111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => hour(1),
	datac => hour(2),
	datad => hour(3),
	combout => \show_hour|Mux7~0_combout\);

-- Location: CLKCTRL_G16
\show_hour|Mux7~0clkctrl\ : cycloneiv_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \show_hour|Mux7~0clkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \show_hour|Mux7~0clkctrl_outclk\);

-- Location: LCCOMB_X16_Y14_N16
\show_hour|Mux0~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|Mux0~0_combout\ = (!hour(3) & (!hour(1) & (hour(2) $ (hour(0)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000100000010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => hour(2),
	datab => hour(3),
	datac => hour(1),
	datad => hour(0),
	combout => \show_hour|Mux0~0_combout\);

-- Location: LCCOMB_X16_Y14_N22
\show_hour|decodeout[0]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|decodeout\(0) = (GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & ((\show_hour|Mux0~0_combout\))) # (!GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & (\show_hour|decodeout\(0)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_hour|decodeout\(0),
	datac => \show_hour|Mux7~0clkctrl_outclk\,
	datad => \show_hour|Mux0~0_combout\,
	combout => \show_hour|decodeout\(0));

-- Location: LCCOMB_X15_Y14_N22
\show_hour|Mux1~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|Mux1~0_combout\ = (hour(2) & (hour(1) $ (hour(0))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0101000010100000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => hour(1),
	datac => hour(2),
	datad => hour(0),
	combout => \show_hour|Mux1~0_combout\);

-- Location: LCCOMB_X15_Y14_N16
\show_hour|decodeout[1]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|decodeout\(1) = (GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & ((\show_hour|Mux1~0_combout\))) # (!GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & (\show_hour|decodeout\(1)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111000011001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_hour|decodeout\(1),
	datac => \show_hour|Mux1~0_combout\,
	datad => \show_hour|Mux7~0clkctrl_outclk\,
	combout => \show_hour|decodeout\(1));

-- Location: LCCOMB_X15_Y14_N4
\show_hour|Mux2~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|Mux2~0_combout\ = (!hour(2) & (hour(1) & !hour(0)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000001010000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => hour(2),
	datac => hour(1),
	datad => hour(0),
	combout => \show_hour|Mux2~0_combout\);

-- Location: LCCOMB_X15_Y14_N0
\show_hour|decodeout[2]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|decodeout\(2) = (GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & ((\show_hour|Mux2~0_combout\))) # (!GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & (\show_hour|decodeout\(2)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_hour|decodeout\(2),
	datac => \show_hour|Mux7~0clkctrl_outclk\,
	datad => \show_hour|Mux2~0_combout\,
	combout => \show_hour|decodeout\(2));

-- Location: LCCOMB_X15_Y14_N8
\show_hour|Mux3~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|Mux3~0_combout\ = (!hour(3) & ((hour(2) & (hour(1) $ (!hour(0)))) # (!hour(2) & (!hour(1) & hour(0)))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0010000100000010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => hour(2),
	datab => hour(3),
	datac => hour(1),
	datad => hour(0),
	combout => \show_hour|Mux3~0_combout\);

-- Location: LCCOMB_X15_Y14_N30
\show_hour|decodeout[3]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|decodeout\(3) = (GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & ((\show_hour|Mux3~0_combout\))) # (!GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & (\show_hour|decodeout\(3)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111101000001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_hour|decodeout\(3),
	datac => \show_hour|Mux7~0clkctrl_outclk\,
	datad => \show_hour|Mux3~0_combout\,
	combout => \show_hour|decodeout\(3));

-- Location: LCCOMB_X15_Y14_N26
\show_hour|Mux4~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|Mux4~0_combout\ = (hour(0)) # ((hour(2) & !hour(1)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100001010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => hour(2),
	datac => hour(1),
	datad => hour(0),
	combout => \show_hour|Mux4~0_combout\);

-- Location: LCCOMB_X15_Y14_N20
\show_hour|decodeout[4]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|decodeout\(4) = (GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & ((\show_hour|Mux4~0_combout\))) # (!GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & (\show_hour|decodeout\(4)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_hour|decodeout\(4),
	datac => \show_hour|Mux7~0clkctrl_outclk\,
	datad => \show_hour|Mux4~0_combout\,
	combout => \show_hour|decodeout\(4));

-- Location: LCCOMB_X15_Y14_N28
\show_hour|Mux5~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|Mux5~0_combout\ = (!hour(3) & ((hour(2) & (hour(1) & hour(0))) # (!hour(2) & ((hour(1)) # (hour(0))))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0011000100010000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => hour(2),
	datab => hour(3),
	datac => hour(1),
	datad => hour(0),
	combout => \show_hour|Mux5~0_combout\);

-- Location: LCCOMB_X15_Y14_N24
\show_hour|decodeout[5]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|decodeout\(5) = (GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & ((\show_hour|Mux5~0_combout\))) # (!GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & (\show_hour|decodeout\(5)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110000001100",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datab => \show_hour|decodeout\(5),
	datac => \show_hour|Mux7~0clkctrl_outclk\,
	datad => \show_hour|Mux5~0_combout\,
	combout => \show_hour|decodeout\(5));

-- Location: LCCOMB_X15_Y14_N14
\show_hour|Mux6~0\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|Mux6~0_combout\ = (hour(3)) # ((hour(2) & ((!hour(0)) # (!hour(1)))) # (!hour(2) & (hour(1))))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111011011111110",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => hour(2),
	datab => hour(1),
	datac => hour(3),
	datad => hour(0),
	combout => \show_hour|Mux6~0_combout\);

-- Location: LCCOMB_X15_Y14_N12
\show_hour|decodeout[6]\ : cycloneiv_lcell_comb
-- Equation(s):
-- \show_hour|decodeout\(6) = (GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & ((!\show_hour|Mux6~0_combout\))) # (!GLOBAL(\show_hour|Mux7~0clkctrl_outclk\) & (\show_hour|decodeout\(6)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000111110101010",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \show_hour|decodeout\(6),
	datac => \show_hour|Mux6~0_combout\,
	datad => \show_hour|Mux7~0clkctrl_outclk\,
	combout => \show_hour|decodeout\(6));

ww_out_sec_l(0) <= \out_sec_l[0]~output_o\;

ww_out_sec_l(1) <= \out_sec_l[1]~output_o\;

ww_out_sec_l(2) <= \out_sec_l[2]~output_o\;

ww_out_sec_l(3) <= \out_sec_l[3]~output_o\;

ww_out_sec_l(4) <= \out_sec_l[4]~output_o\;

ww_out_sec_l(5) <= \out_sec_l[5]~output_o\;

ww_out_sec_l(6) <= \out_sec_l[6]~output_o\;

ww_out_sec_h(0) <= \out_sec_h[0]~output_o\;

ww_out_sec_h(1) <= \out_sec_h[1]~output_o\;

ww_out_sec_h(2) <= \out_sec_h[2]~output_o\;

ww_out_sec_h(3) <= \out_sec_h[3]~output_o\;

ww_out_sec_h(4) <= \out_sec_h[4]~output_o\;

ww_out_sec_h(5) <= \out_sec_h[5]~output_o\;

ww_out_sec_h(6) <= \out_sec_h[6]~output_o\;

ww_out_min_l(0) <= \out_min_l[0]~output_o\;

ww_out_min_l(1) <= \out_min_l[1]~output_o\;

ww_out_min_l(2) <= \out_min_l[2]~output_o\;

ww_out_min_l(3) <= \out_min_l[3]~output_o\;

ww_out_min_l(4) <= \out_min_l[4]~output_o\;

ww_out_min_l(5) <= \out_min_l[5]~output_o\;

ww_out_min_l(6) <= \out_min_l[6]~output_o\;

ww_out_min_h(0) <= \out_min_h[0]~output_o\;

ww_out_min_h(1) <= \out_min_h[1]~output_o\;

ww_out_min_h(2) <= \out_min_h[2]~output_o\;

ww_out_min_h(3) <= \out_min_h[3]~output_o\;

ww_out_min_h(4) <= \out_min_h[4]~output_o\;

ww_out_min_h(5) <= \out_min_h[5]~output_o\;

ww_out_min_h(6) <= \out_min_h[6]~output_o\;

ww_out_hour(0) <= \out_hour[0]~output_o\;

ww_out_hour(1) <= \out_hour[1]~output_o\;

ww_out_hour(2) <= \out_hour[2]~output_o\;

ww_out_hour(3) <= \out_hour[3]~output_o\;

ww_out_hour(4) <= \out_hour[4]~output_o\;

ww_out_hour(5) <= \out_hour[5]~output_o\;

ww_out_hour(6) <= \out_hour[6]~output_o\;
END structure;


