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

    while (true) {
#pragma HLS PIPELINE II=1

        // Read inputs
        in_val = input_signal.read();
        car_val = carrier.read();

        // Multiply
        float x = *((float*) &in_val.data);
        float c = *((float*) &car_val.data);
        float y = x * c;

        out_val.data = *((ap_uint<32>*) &y);

        // Pass through control signals
        out_val.last = in_val.last;

        output_signal.write(out_val);

        // Exit condition
        if (in_val.last == 1)
            break;
    }
}
