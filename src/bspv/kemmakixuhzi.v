//
//	model    : uhzi kemmakix
//	function : simulasion input output data
//	created  : 18:47 2004/02/21
//


module uhzi(data_out,req_out,ack_in, switch,reset);
input	ack_in,reset,switch;
output	data_out,req_out;
	reg	data;
	reg	r,run;
	reg	[9:0]	count;
	reg	mem[0:1023];

	parameter REQ_DELAY = 10;

	always @(switch or ack_in or negedge reset) begin
		if(switch)begin
			if(mem[count]===1'bx);
			else begin
				r=1'b1;
				run<=1'b1;
			end
		end
		if(!reset)begin
			data=mem[0];
			r=1'b0;
			count<=0;
			run<=0;
		end else if(run==1 & mem[count]===1'bx) begin
			run<=0;
			count<=count-1;
		end else if (!ack_in & switch)begin
			r=1'b1;
			data=mem[count];
		end else if (ack_in  & switch)begin
			r=1'b0;
			count<=count+1'b1;		/* adress counter */			
		end 
	end
	assign #(REQ_DELAY) req_out=r&run;
	assign data_out=data;
endmodule


module kemmakix(data_in,req_in,ack_out,switch,reset);
input	data_in,req_in,switch, reset;
output	ack_out;
	reg	[9:0]	count;
	reg	mem[0:1023];

	parameter ACK_DELAY = 10;

	assign #(ACK_DELAY)ack_out = switch ? req_in : 0;
	always @(posedge ack_out or negedge reset) begin
		if(!reset) begin
			count<=0;
		end else begin
			mem[count]<=data_in;
			count<=count+1;
		end
	end
endmodule
