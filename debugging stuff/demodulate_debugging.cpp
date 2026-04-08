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


// *********************** UPDATED VERSION THAT WE SHOULD TRY ON WED ********************
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <ap_int.h>

typedef ap_axiu<32, 0, 0, 0> axis_t;

union float_bits {
    float f;
    ap_uint<32> u;
};

void demodulate(
    hls::stream<axis_t> &input_signal,
    hls::stream<axis_t> &carrier,
    hls::stream<axis_t> &output_signal
) {
#pragma HLS INTERFACE axis port=input_signal
#pragma HLS INTERFACE axis port=carrier
#pragma HLS INTERFACE axis port=output_signal
#pragma HLS INTERFACE s_axilite port=return bundle=control

    for (int i = 0; i < 1024; i++) {
#pragma HLS PIPELINE II=1
        axis_t in_val = input_signal.read();
        axis_t car_val = carrier.read();
        axis_t out_val;

        float_bits a, b, y;
        a.u = in_val.data;
        b.u = car_val.data;
        y.f = a.f * b.f;

        out_val.data = y.u;
        out_val.keep = in_val.keep;
        out_val.strb = in_val.strb;
        out_val.last = in_val.last;

        output_signal.write(out_val);
    }
}



// *********** NEW VERSION WITH ONLY ONE INPUT STREAM ***************

#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <ap_int.h>

typedef ap_axiu<64, 0, 0, 0> axis_in_t;
typedef ap_axiu<32, 0, 0, 0> axis_out_t;

union float_bits {
    float f;
    ap_uint<32> u;
};

void demodulate_packed(
    hls::stream<axis_in_t> &in_stream,
    hls::stream<axis_out_t> &out_stream
) {
#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS INTERFACE s_axilite port=return bundle=control

    for (int i = 0; i < 1024; i++) {
#pragma HLS PIPELINE II=1
        axis_in_t in_val = in_stream.read();
        axis_out_t out_val;

        ap_uint<64> packed = in_val.data;
        ap_uint<32> sig_bits = packed.range(31, 0);
        ap_uint<32> car_bits = packed.range(63, 32);

        float_bits sig, car, out;
        sig.u = sig_bits;
        car.u = car_bits;
        out.f = sig.f * car.f;

        out_val.data = out.u;
        out_val.keep = 0xF;
        out_val.strb = 0xF;
        out_val.last = in_val.last;

        out_stream.write(out_val);
    }
}