/*******************************************************************************

    Copyright 2012 Ben Wojtowicz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************

    File: LTE_fdd_dl_fs_samp_buf.h

    Description: Contains all the definitions for the LTE FDD DL File Scanner
                 sample buffer block.

    Revision History
    ----------    -------------    --------------------------------------------
    03/23/2012    Ben Wojtowicz    Created file.
    08/19/2012    Ben Wojtowicz    Added states and state memory and added
                                   decoding of all SIBs.

*******************************************************************************/
#ifndef __LTE_FDD_DL_FS_SAMP_BUF_H__
#define __LTE_FDD_DL_FS_SAMP_BUF_H__


/*******************************************************************************
                              INCLUDES
*******************************************************************************/

#include "gr_sync_block.h"
#include "liblte_phy.h"
#include "liblte_rrc.h"

/*******************************************************************************
                              DEFINES
*******************************************************************************/

#define LTE_FDD_DL_FS_SAMP_BUF_SIZE (307200*10)

/*******************************************************************************
                              FORWARD DECLARATIONS
*******************************************************************************/

class LTE_fdd_dl_fs_samp_buf;

/*******************************************************************************
                              TYPEDEFS
*******************************************************************************/

typedef boost::shared_ptr<LTE_fdd_dl_fs_samp_buf> LTE_fdd_dl_fs_samp_buf_sptr;

typedef enum{
    LTE_FDD_DL_FS_SAMP_BUF_STATE_COARSE_TIMING_SEARCH = 0,
    LTE_FDD_DL_FS_SAMP_BUF_STATE_PSS_AND_FINE_TIMING_SEARCH,
    LTE_FDD_DL_FS_SAMP_BUF_STATE_SSS_SEARCH,
    LTE_FDD_DL_FS_SAMP_BUF_STATE_BCH_DECODE,
    LTE_FDD_DL_FS_SAMP_BUF_STATE_PDSCH_DECODE_SIB1,
    LTE_FDD_DL_FS_SAMP_BUF_STATE_PDSCH_DECODE_SI_GENERIC,
}LTE_FDD_DL_FS_SAMP_BUF_STATE_ENUM;

/*******************************************************************************
                              CLASS DECLARATIONS
*******************************************************************************/

LTE_fdd_dl_fs_samp_buf_sptr LTE_fdd_dl_fs_make_samp_buf ();
class LTE_fdd_dl_fs_samp_buf : public gr_sync_block
{
public:
    ~LTE_fdd_dl_fs_samp_buf();

    int32 work(int32                      ninput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star       &output_items);

private:
    friend LTE_fdd_dl_fs_samp_buf_sptr LTE_fdd_dl_fs_make_samp_buf();

    LTE_fdd_dl_fs_samp_buf();

    // LTE library
    LIBLTE_PHY_STRUCT                *phy_struct;
    LIBLTE_RRC_MSG_STRUCT             rrc_msg;
    LIBLTE_RRC_MIB_STRUCT             mib;
    LIBLTE_RRC_BCCH_DLSCH_MSG_STRUCT  bcch_dlsch_msg;

    // Sample buffer
    float  *i_buf;
    float  *q_buf;
    uint32  samp_buf_w_idx;
    uint32  samp_buf_r_idx;
    bool    last_samp_was_i;

    // Variables
    LTE_FDD_DL_FS_SAMP_BUF_STATE_ENUM state;
    float                             freq_offset;
    float                             phich_res;
    uint32                            N_rb_dl;
    uint32                            FFT_pad_size;
    uint32                            sfn;
    uint32                            N_sfr;
    uint32                            N_id_cell;
    uint32                            N_id_1;
    uint32                            N_id_2;
    uint8                             N_ant;
    uint8                             prev_si_value_tag;
    bool                              prev_si_value_tag_valid;
    bool                              mib_printed;
    bool                              sib1_printed;
    bool                              sib2_printed;
    bool                              sib3_printed;
    bool                              sib4_printed;
    bool                              sib8_printed;

    // Helpers
    void copy_input_to_samp_buf(const int8 *in, int32 ninput_items);
    void freq_shift(uint32 start_idx, uint32 num_samps, float freq_offset);
    void print_mib(LIBLTE_RRC_MIB_STRUCT *mib);
    void print_sib1(LIBLTE_RRC_SYS_INFO_BLOCK_TYPE_1_STRUCT *sib1);
    void print_sib2(LIBLTE_RRC_SYS_INFO_BLOCK_TYPE_2_STRUCT *sib2);
    void print_sib3(LIBLTE_RRC_SYS_INFO_BLOCK_TYPE_3_STRUCT *sib3);
    void print_sib4(LIBLTE_RRC_SYS_INFO_BLOCK_TYPE_4_STRUCT *sib4);
    void print_sib8(LIBLTE_RRC_SYS_INFO_BLOCK_TYPE_8_STRUCT *sib8);
};

#endif /* __LTE_FDD_DL_FS_SAMP_BUF_H__ */
