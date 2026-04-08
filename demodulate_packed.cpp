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