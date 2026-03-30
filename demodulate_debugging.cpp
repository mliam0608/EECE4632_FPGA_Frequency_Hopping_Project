#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef ap_axiu<32, 0, 0, 0> axis_t;

void demodulate(
    hls::stream<axis_t> &input_signal,
    hls::stream<axis_t> &carrier,
    hls::stream<axis_t> &output_signal
) {
#pragma HLS INTERFACE axis port=input_signal
#pragma HLS INTERFACE axis port=carrier
#pragma HLS INTERFACE axis port=output_signal

#pragma HLS INTERFACE s_axilite port=return bundle=control

    axis_t in_val, car_val, out_val;

   // Use a fixed loop size matching your N
    for (int i = 0; i < 1024; i++) { 
#pragma HLS PIPELINE II=1
        axis_t in_val = input_signal.read();
        axis_t car_val = carrier.read();
        axis_t out_val;

        float x = *((float*) &in_val.data);
        float c = *((float*) &car_val.data);
        float y = x * c;

        out_val.data = *((ap_uint<32>*) &y);

        // FORCE these sidebands for the DMA
        out_val.keep = 0xF; 
        out_val.strb = 0xF;
        
        // MANUALLY assert last on the final iteration
        out_val.last = (i == 1023) ? 1 : 0; 

        output_signal.write(out_val);
    }
}