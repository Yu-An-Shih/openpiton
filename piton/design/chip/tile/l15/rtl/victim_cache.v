//`include "l15.tmp.h"

//`include "define.tmp.h"
//`ifdef DEFAULT_NETTYPE_NONE
//`default_nettype none
//`endif

// `L15_CACHE_TAG_WIDTH == 29
// `L15_CACHE_INDEX_WIDTH == 7

//`define VC_ADDR_WIDTH (`L15_CACHE_TAG_WIDTH+`L15_CACHE_INDEX_WIDTH)
//`define VC_NUM_ENTRIES 16


module victim_cache (
    input wire clk,
    input wire rst_n,
    
    // S3
    input wire l15_vc_store_evict_val_s3,
    input wire [`VC_ADDR_WIDTH-1:0] l15_vc_store_evict_addr_s3,
    input wire [`L15_CACHELINE_WIDTH-1:0] l15_vc_store_evict_data_s3
);

reg [`VC_ADDR_WIDTH-1:0] vc_addr [`VC_NUM_ENTRIES-1:0], vc_addr_next [`VC_NUM_ENTRIES-1:0];
reg [`L15_MESI_STATE_WIDTH-1:0] vc_mesi [`VC_NUM_ENTRIES-1:0], vc_mesi_next [`VC_NUM_ENTRIES-1:0];
reg [`L15_CACHELINE_WIDTH-1:0] vc_data [`VC_NUM_ENTRIES-1:0], vc_data_next [`VC_NUM_ENTRIES-1:0];

// S3
reg [`VC_NUM_ENTRIES_LOG2-1:0] store_evict_cntr_s3, store_evict_cntr_s3_next;

integer i;
always @(*) begin
    for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
        vc_addr_next[i] = vc_addr[i];
        vc_mesi_next[i] = vc_mesi[i];
        vc_data_next[i] = vc_data[i];
    end
    store_evict_cntr_s3_next = store_evict_cntr_s3;

    // store evict data (S3)
    if (l15_vc_store_evict_val_s3) begin

        vc_addr_next[store_evict_cntr_s3] = l15_vc_store_evict_addr_s3;
        vc_data_next[store_evict_cntr_s3] = l15_vc_store_evict_data_s3;
        vc_mesi_next[store_evict_cntr_s3] = `L15_MESI_STATE_E;
        // TODO: handle conflicts with S1, S2
        // TODO: what if vc_mesi[store_evict_cntr_s3] == `L15_MESI_STATE_M?

        store_evict_cntr_s3_next = store_evict_cntr_s3 + 1;
    end
end

always @(posedge clk) begin
    if (!rst_n) begin
        for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
            vc_addr[i] <= 0;
            vc_mesi[i] <= `L15_MESI_STATE_I;
            vc_data[i] <= 0;
        end
        store_evict_cntr_s3 <= 0;
    end else begin
        for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
            vc_addr[i] <= vc_addr_next[i];
            vc_mesi[i] <= vc_mesi_next[i];
            vc_data[i] <= vc_data_next[i];
        end
        store_evict_cntr_s3 <= store_evict_cntr_s3_next;
    end
end

endmodule
