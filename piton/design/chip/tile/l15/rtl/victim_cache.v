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
    // S1
    input wire check_val_i,
    input wire check_rw_i,
    input wire [`VC_ADDR_WIDTH-1:0] check_addr_i,

    output wire [`VC_NUM_ENTRIES_LOG2-1:0] match_index_o,
    output wire [`L15_MESI_STATE_WIDTH-1:0] match_mesi_o,
    // S2
    input wire fetch_val_i,
    input wire fetch_rw_i,
    input wire [`VC_NUM_ENTRIES_LOG2-1:0] fetch_index_i,
    input wire [`L15_CACHELINE_WIDTH-1:0] write_mask_i,
    input wire [`L15_CACHELINE_WIDTH-1:0] write_data_i,
    input wire mesi_write_val_i,
    input wire [`L15_MESI_STATE_WIDTH-1:0] mesi_write_state_i,

    output wire [`L15_CACHELINE_WIDTH-1:0] fetch_data_o,
    // S3
    input wire store_evict_val_i,
    input wire [`VC_ADDR_WIDTH-1:0] store_evict_addr_i,
    input wire [`L15_CACHELINE_WIDTH-1:0] store_evict_data_i
);

reg [`VC_ADDR_WIDTH-1:0] vc_addr [`VC_NUM_ENTRIES-1:0], vc_addr_next [`VC_NUM_ENTRIES-1:0];
reg [`L15_MESI_STATE_WIDTH-1:0] vc_mesi [`VC_NUM_ENTRIES-1:0], vc_mesi_next [`VC_NUM_ENTRIES-1:0];
reg [`L15_CACHELINE_WIDTH-1:0] vc_data [`VC_NUM_ENTRIES-1:0], vc_data_next [`VC_NUM_ENTRIES-1:0];
// S1
reg [`VC_NUM_ENTRIES_LOG2-1:0] match_index, match_index_next;
reg [`L15_MESI_STATE_WIDTH-1:0] match_mesi, match_mesi_next;
// S2
reg [`L15_CACHELINE_WIDTH-1:0] fetch_data, fetch_data_next;
// S3
reg [`VC_NUM_ENTRIES_LOG2-1:0] evict_cntr, evict_cntr_next;

integer i;
always @(*) begin
    for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
        vc_addr_next[i] = vc_addr[i];
        vc_mesi_next[i] = vc_mesi[i];
        vc_data_next[i] = vc_data[i];
    end
    evict_cntr_next = evict_cntr;

    // cacheable write (S2)
    if (fetch_val_i && fetch_rw_i == `L15_DTAG_RW_WRITE) begin
        vc_data_next[fetch_index_i] = (write_data_i & write_mask_i) | (vc_data[fetch_index_i] & ~write_mask_i);
    end

    // mesi write (S2)
    if (mesi_write_val_i) begin
        vc_mesi_next[fetch_index_i] = mesi_write_state_i;
    end

    // store evict data (S3)
    if (store_evict_val_i) begin
        // handle conflicts
        if ( ( ((match_index_next == evict_cntr) && (match_mesi_next != `L15_MESI_STATE_I))
            && (fetch_val_i && (fetch_index_i == evict_cntr + 1)) )
          || ( (fetch_val_i && (fetch_index_i == evict_cntr)) 
            && ((match_index_next == evict_cntr + 1) && (match_mesi_next != `L15_MESI_STATE_I)) )
        ) begin
            vc_addr_next[evict_cntr + 2] = store_evict_addr_i;
            vc_data_next[evict_cntr + 2] = store_evict_data_i;
            vc_mesi_next[evict_cntr + 2] = `L15_MESI_STATE_E;
            evict_cntr_next = evict_cntr + 3;
        end 
        else if ( ((match_index_next == evict_cntr) && (match_mesi_next != `L15_MESI_STATE_I)) 
               || (fetch_val_i && (fetch_index_i == evict_cntr))
        ) begin
            vc_addr_next[evict_cntr + 1] = store_evict_addr_i;
            vc_data_next[evict_cntr + 1] = store_evict_data_i;
            vc_mesi_next[evict_cntr + 1] = `L15_MESI_STATE_E;
            evict_cntr_next = evict_cntr + 2;
        end else begin
            vc_addr_next[evict_cntr] = store_evict_addr_i;
            vc_data_next[evict_cntr] = store_evict_data_i;
            vc_mesi_next[evict_cntr] = `L15_MESI_STATE_E;
            // TODO: what if vc_mesi[evict_cntr] == `L15_MESI_STATE_M?

            evict_cntr_next = evict_cntr + 1;
        end
    end
    
    if (store_evict_val_i) begin
        evict_cntr_next = evict_cntr + 1;
    end
end

always @(posedge clk) begin
    if (!rst_n) begin
        for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
            vc_addr[i] <= 0;
            vc_mesi[i] <= `L15_MESI_STATE_I;
            vc_data[i] <= 0;
        end
        evict_cntr <= 0;
    end else begin
        for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
            vc_addr[i] <= vc_addr_next[i];
            vc_mesi[i] <= vc_mesi_next[i];
            vc_data[i] <= vc_data_next[i];
        end
        evict_cntr <= evict_cntr_next;
    end
end

// S1
assign match_index_o = match_index;
assign match_mesi_o = match_mesi;

always @(*) begin
    match_index_next = 0;
    match_mesi_next = `L15_MESI_STATE_I;

    if (check_val_i) begin
        for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
            if ((check_rw_i == `L15_DTAG_RW_READ) && (check_addr_i == vc_addr[i])) begin
                match_index_next = i;
                match_mesi_next = vc_mesi[i];
            end
        end
    end
end

always @(posedge clk) begin
    if (!rst_n) begin
        match_index <= 0;
        match_mesi <= `L15_MESI_STATE_I;
    end else begin
        match_index <= match_index_next;
        match_mesi <= match_mesi_next;
    end
end

// S2
assign fetch_data_o = fetch_data;

always @(*) begin
    fetch_data_next = 0;

    if (fetch_val_i && (fetch_rw_i == `L15_DTAG_RW_READ)) begin
        fetch_data_next = vc_data[fetch_index_i];
    end
end

always @(posedge clk) begin
    if (!rst_n) begin
        fetch_data <= 0;
    end else begin
        fetch_data <= fetch_data_next;
    end
end

endmodule

/*module victim_cache (
    input wire clk,
    input wire rst_n,

    input wire valid_i,
    
    input wire read_valid_i,
    input wire [`VC_ADDR_WIDTH-1:0] read_addr_i,
    
    //input wire write_valid,

    output wire read_hit_o,
    output wire [`L15_CACHELINE_WIDTH-1:0] read_data_o
);

reg [`VC_ADDR_WIDTH-1:0] vc_addr [`VC_NUM_ENTRIES-1:0];
reg [`L15_MESI_STATE_WIDTH-1:0] vc_mesi [`VC_NUM_ENTRIES-1:0];
reg [`L15_CACHELINE_WIDTH-1:0] vc_data [`VC_NUM_ENTRIES-1:0];

reg read_hit [`VC_NUM_ENTRIES-1:0];
reg [`L15_CACHELINE_WIDTH-1:0] read_data;

assign read_data_o = read_data;

integer i;
always @(posedge clk) begin
    for (i = 0; i < `VC_NUM_ENTRIES; i = i + 1) begin
        if (!rst_n) begin
            vc_addr[i] = 0;
            vc_mesi[i] = `L15_MESI_STATE_I;
            vc_data[i] = 0;
        end else begin
            if (valid_i) begin
                if (read_valid_i && (read_addr_i == vc_addr[i]) && (vc_mesi[i] != `L15_MESI_STATE_I)) begin
                    read_hit[i] = 1'b1;
                    read_data = vc_data[i];
                end
            end
        end
    end
end
endmodule*/
