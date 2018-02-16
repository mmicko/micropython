// Wrapper for 5k hard IP
module ip_wrapper_up5k(
  input clock,
  input reset,
  input  [23:0] address,
  input  [31:0] write_data,
  output [31:0] read_data,
  input  [3:0]  wstrb,
  input         valid,
  output reg ready,
  
  output [2:0]  pwm,
  
  inout i2c_sda,
  inout i2c_scl
);

wire led_en, i2c_en, ctrl_en;

assign led_en = (address[23:16] == 8'h00);
assign i2c_en = (address[23:16] == 8'h01);
assign ctrl_en = (address[23:16] == 8'h10);

wire [7:0] i2c_read_data;
wire i2c_ack;

reg [31:0] rdata_reg;
reg ready;

reg [7:0] ctrl_reg;

always @(posedge clock)
begin
  if(!valid)
    ready <= 1'b0;
  if(!ready && led_en && valid) 
  begin
    ready <= 1'b1;
    rdata_reg <= 32'h0;
  end else if(!ready && ctrl_en && valid)
  begin
    ready <= 1'b1;
    if(wstrb[0])
      ctrl_reg <= write_data;
    rdata_reg <= {24'h0, ctrl_reg};
  end else if(!ready && i2c_en && i2c_ack && valid)
  begin
    ready <= 1'b1;
    rdata_reg <= i2c_read_data;
  end
end

assign read_data = rdata_reg;

SB_LEDDA_IP ledda_i (
  .LEDDCS(led_en),
  .LEDDCLK(clock),
  .LEDDDAT7(write_data[7]),
  .LEDDDAT6(write_data[6]),
  .LEDDDAT5(write_data[5]),
  .LEDDDAT4(write_data[4]),
  .LEDDDAT3(write_data[3]),
  .LEDDDAT2(write_data[2]),
  .LEDDDAT1(write_data[1]),
  .LEDDDAT0(write_data[0]),
  .LEDDADDR3(address[5]),
  .LEDDADDR2(address[4]),
  .LEDDADDR1(address[3]),
  .LEDDADDR0(address[2]),
  .LEDDDEN(led_en && valid && !ready && wstrb[0]),
  .LEDDEXE(ctrl_reg[0]),
  .PWMOUT0(pwm[0]),
  .PWMOUT1(pwm[1]),
  .PWMOUT2(pwm[2])
);

wire sdai, sdao, sdaoe, scli, sclo, scloe;

SB_I2C #(
  .I2C_SLAVE_INIT_ADDR("0b1111100001"),
  .BUS_ADDR74("0b0001")
) i2c_i (
  .SBCLKI(clock),
  .SBRWI(wstrb[0]),
  .SBSTBI(valid && i2c_en && !ready),
  .SBADRI0(address[2]),
  .SBADRI1(address[3]),
  .SBADRI2(address[4]),
  .SBADRI3(address[5]),
  .SBADRI4(address[6]),
  .SBADRI5(address[7]),
  .SBADRI6(address[8]),
  .SBADRI7(address[9]),
  .SBDATI0(write_data[0]),
  .SBDATI1(write_data[1]),
  .SBDATI2(write_data[2]),
  .SBDATI3(write_data[3]),
  .SBDATI4(write_data[4]),
  .SBDATI5(write_data[5]),
  .SBDATI6(write_data[6]),
  .SBDATI7(write_data[7]),
  .SBDATO0(i2c_read_data[0]),
  .SBDATO1(i2c_read_data[1]),
  .SBDATO2(i2c_read_data[2]),
  .SBDATO3(i2c_read_data[3]),
  .SBDATO4(i2c_read_data[4]),
  .SBDATO5(i2c_read_data[5]),
  .SBDATO6(i2c_read_data[6]),
  .SBDATO7(i2c_read_data[7]),
  .SBACKO(i2c_ack),
  .I2CIRQ(),
  .I2CWKUP(),
  .SCLI(scli),
  .SCLO(sclo),
  .SCLOE(scloe),
  .SDAI(sdai),
  .SDAO(sdao),
  .SDAOE(sdaoe)
);

SB_IO #(
  .PIN_TYPE(6'b101001),
  .PULLUP(1'b1)
) scl_io (
  .PACKAGE_PIN(i2c_scl),
  .OUTPUT_ENABLE(scloe),
  .D_OUT_0(sclo),
  .D_IN_0(scli)
);


SB_IO #(
  .PIN_TYPE(6'b101001),
  .PULLUP(1'b1)
) sda_io (
  .PACKAGE_PIN(i2c_sda),
  .OUTPUT_ENABLE(sdaoe),
  .D_OUT_0(sdao),
  .D_IN_0(sdai)
);
endmodule