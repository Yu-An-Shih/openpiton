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
    input clk,
    input rst_n,
    // S1
    input l15_vc_val_s1,
    input l15_vc_rw_s1,
    input [`VC_ADDR_WIDTH-1:0] l15_vc_addr_s1,
    input [`L15_UNPARAM_127_0] l15_vc_write_mask_s1,
    input [`L15_CACHELINE_WIDTH-1:0] l15_vc_write_data_s1,
    // S2
    output [`VC_NUM_ENTRIES_LOG2-1:0] vc_l15_index_s2,
    output [`L15_MESI_STATE_WIDTH-1:0] vc_l15_mesi_s2,
    output [`L15_CACHELINE_WIDTH-1:0] vc_l15_data_s2,
    // S3
    input l15_vc_store_evict_val_s3,
    input [`VC_ADDR_WIDTH-1:0] l15_vc_store_evict_addr_s3,
    input [`L15_CACHELINE_WIDTH-1:0] l15_vc_store_evict_data_s3
);

reg [`VC_ADDR_WIDTH-1:0] vc_addr [`VC_NUM_ENTRIES-1:0], vc_addr_next [`VC_NUM_ENTRIES-1:0];
reg [`L15_MESI_STATE_WIDTH-1:0] vc_mesi [`VC_NUM_ENTRIES-1:0], vc_mesi_next [`VC_NUM_ENTRIES-1:0];
reg [`L15_CACHELINE_WIDTH-1:0] vc_data [`VC_NUM_ENTRIES-1:0], vc_data_next [`VC_NUM_ENTRIES-1:0];
// S1
//reg vc_hit_s2, vc_hit_s2_next;
reg [`VC_NUM_ENTRIES_LOG2-1:0] match_index_s2, match_index_s2_next;
reg [`L15_MESI_STATE_WIDTH-1:0] match_mesi_s2, match_mesi_s2_next;
reg [`L15_CACHELINE_WIDTH-1:0] match_data_s2, match_data_s2_next;
// S3
reg [`VC_NUM_ENTRIES_LOG2-1:0] store_evict_cntr_s3, store_evict_cntr_s3_next;

assign vc_l15_index_s2 = match_index_s2;
assign vc_l15_mesi_s2 = match_mesi_s2;
assign vc_l15_data_s2 = match_data_s2;

integer i;
always @(*) begin
    for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
        vc_addr_next[i] = vc_addr[i];
        vc_mesi_next[i] = vc_mesi[i];
        vc_data_next[i] = vc_data[i];
    end
    //vc_hit_s2_next = 0;
    match_index_s2_next = 0;
    match_mesi_s2_next = 0;
    match_data_s2_next = 0;
    store_evict_cntr_s3_next = store_evict_cntr_s3;

    // S1: read or write data
    if (l15_vc_val_s1) begin
        case (l15_vc_rw_s1)
            `L15_DTAG_RW_READ:
            begin
                for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
                    if ( (vc_mesi[i] != `L15_MESI_STATE_I) && (l15_vc_addr_s1 == vc_addr[i]) 
                      && ( !l15_vc_store_evict_val_s3 || (i != store_evict_cntr_s3) ) ) // handle conflict with S3
                    begin
                        //vc_hit_s2_next = 1'b1;
                        match_index_s2_next = i;
                        match_mesi_s2_next = vc_mesi[i];
                        match_data_s2_next = vc_data[i];
                    end
                end
            end
            `L15_DTAG_RW_WRITE:
            begin
                for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
                    if ( (vc_mesi[i] != `L15_MESI_STATE_I) && (l15_vc_addr_s1 == vc_addr[i]) 
                      && ( !l15_vc_store_evict_val_s3 || (i != store_evict_cntr_s3) ) ) // handle conflict with S3
                    begin
                        //vc_hit_s2_next = 1'b1;
                        match_index_s2_next = i;
                        vc_mesi_next[i] = `L15_MESI_STATE_M;
                        vc_data_next[i] = (l15_vc_write_data_s1 & l15_vc_write_mask_s1) | (vc_data[i] & ~l15_vc_write_mask_s1);

                        match_mesi_s2_next = vc_mesi_next[i];
                        match_data_s2_next = vc_data_next[i];
                    end
                end
            end
        endcase
    end

    // S3: store evict data
    if (l15_vc_store_evict_val_s3) begin
        
        if (vc_mesi[store_evict_cntr_s3] != `L15_MESI_STATE_M) begin
            vc_addr_next[store_evict_cntr_s3] = l15_vc_store_evict_addr_s3;
            vc_data_next[store_evict_cntr_s3] = l15_vc_store_evict_data_s3;
            vc_mesi_next[store_evict_cntr_s3] = `L15_MESI_STATE_E;
        end
        store_evict_cntr_s3_next = store_evict_cntr_s3 + 1;
        // TODO: evict dirty data from VC
    end
end

always @(posedge clk) begin
    if (!rst_n) begin
        for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
            vc_addr[i] <= 0;
            vc_mesi[i] <= `L15_MESI_STATE_I;
            vc_data[i] <= 0;
        end
        //vc_hit_s2 <= 0;
        match_index_s2 <= 0;
        match_mesi_s2 <= 0;
        match_data_s2 <= 0;
        store_evict_cntr_s3 <= 0;
    end else begin
        for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
            vc_addr[i] <= vc_addr_next[i];
            vc_mesi[i] <= vc_mesi_next[i];
            vc_data[i] <= vc_data_next[i];
        end
        //vc_hit_s2 <= vc_hit_s2_next;
        match_index_s2 <= match_index_s2_next;
        match_mesi_s2 <= match_mesi_s2_next;
        match_data_s2 <= match_data_s2_next;
        store_evict_cntr_s3 <= store_evict_cntr_s3_next;
    end
end

endmodule
