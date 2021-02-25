/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2020 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#ifndef SRSLTE_UE_MAC_INTERFACES_H
#define SRSLTE_UE_MAC_INTERFACES_H

namespace srsue {

class mac_interface_phy_lte
{
public:
  typedef struct {
    uint32_t nof_mbsfn_services;
  } mac_phy_cfg_mbsfn_t;

  typedef struct {
    uint32_t tbs;
    bool     ndi;
    bool     ndi_present;
    int      rv;
  } mac_tb_t;

  typedef struct {
    mac_tb_t tb[SRSLTE_MAX_TB];
    uint32_t pid;
    uint16_t rnti;
    bool     is_sps_release;
    uint32_t tti;
  } mac_grant_dl_t;

  typedef struct {
    mac_tb_t tb;
    uint32_t pid;
    uint16_t rnti;
    bool     phich_available;
    bool     hi_value;
    bool     is_rar;
    uint32_t tti_tx;
  } mac_grant_ul_t;

  typedef struct {
    bool     enabled;
    uint32_t rv;
    uint8_t* payload;
    union {
      srslte_softbuffer_rx_t* rx;
      srslte_softbuffer_tx_t* tx;
    } softbuffer;
  } tb_action_t;

  typedef struct {
    tb_action_t tb[SRSLTE_MAX_TB];

    bool generate_ack;
  } tb_action_dl_t;

  typedef struct {
    tb_action_t tb;
    uint32_t    current_tx_nb;
    bool        expect_ack;
    bool        is_rar;
  } tb_action_ul_t;

  /* Query the MAC for the current RNTI to look for
   */
  virtual uint16_t get_dl_sched_rnti(uint32_t tti) = 0;
  virtual uint16_t get_ul_sched_rnti(uint32_t tti) = 0;

  /* Indicate reception of UL dci.
   * payload_ptr points to memory where MAC PDU must be written by MAC layer */
  virtual void new_grant_ul(uint32_t cc_idx, mac_grant_ul_t grant, tb_action_ul_t* action) = 0;

  /* Indicate reception of DL dci. */
  virtual void new_grant_dl(uint32_t cc_idx, mac_grant_dl_t grant, tb_action_dl_t* action) = 0;

  /* Indicate successful decoding of PDSCH AND PCH TB. */
  virtual void tb_decoded(uint32_t cc_idx, mac_grant_dl_t grant, bool ack[SRSLTE_MAX_CODEWORDS]) = 0;

  /* Indicate successful decoding of BCH TB through PBCH */
  virtual void bch_decoded_ok(uint32_t cc_idx, uint8_t* payload, uint32_t len) = 0;

  /* Indicate successful decoding of MCH TB through PMCH */
  virtual void mch_decoded(uint32_t len, bool crc) = 0;

  /* Obtain action for a new MCH subframe. */
  virtual void new_mch_dl(const srslte_pdsch_grant_t& phy_grant, tb_action_dl_t* action) = 0;

  /* Communicate the number of mbsfn services available  */
  virtual void set_mbsfn_config(uint32_t nof_mbsfn_services) = 0;
};

class mac_interface_rrc_common
{
public:
  // Class to handle UE specific RNTIs between RRC and MAC
  typedef struct {
    uint16_t crnti;
    uint16_t rar_rnti;
    uint16_t temp_rnti;
    uint16_t tpc_rnti;
    uint16_t sps_rnti;
    uint64_t contention_id;
  } ue_rnti_t;
};

class mac_interface_rrc : public mac_interface_rrc_common
{
public:
  /* Instructs the MAC to start receiving BCCH */
  virtual void bcch_start_rx(int si_window_start, int si_window_length) = 0;
  virtual void bcch_stop_rx()                                           = 0;

  /* Instructs the MAC to start receiving PCCH */
  virtual void pcch_start_rx() = 0;

  /* RRC configures a logical channel */
  virtual void setup_lcid(uint32_t lcid, uint32_t lcg, uint32_t priority, int PBR_x_tti, uint32_t BSD) = 0;

  /* Instructs the MAC to start receiving an MCH */
  virtual void mch_start_rx(uint32_t lcid) = 0;

  /* Set entire MAC config */
  virtual void set_config(srslte::mac_cfg_t& mac_cfg) = 0;

  /* Update SR config only for PUCCH release */
  virtual void set_config(srslte::sr_cfg_t& sr_cfg) = 0;

  virtual void set_rach_ded_cfg(uint32_t preamble_index, uint32_t prach_mask) = 0;

  virtual void get_rntis(ue_rnti_t* rntis)                      = 0;
  virtual void set_contention_id(uint64_t uecri)                = 0;
  virtual void set_ho_rnti(uint16_t crnti, uint16_t target_pci) = 0;

  virtual void reconfiguration(const uint32_t& cc_idx, const bool& enable) = 0;
  virtual void reset()                                                     = 0;
};

} // namespace srsue

#endif // SRSLTE_UE_MAC_INTERFACES_H
