#pragma once

#include "netsocket/EMAC.h"
#include "rtos/Thread.h"
#include "rtos/Semaphore.h"
#include "hal_data.h"
#include "r_ether.h"
#include "r_ether_phy.h"

class RA_EMAC : public EMAC
{
public:
    static RA_EMAC &get_instance()
    {
        static RA_EMAC emac;
        return emac;
    }

    RA_EMAC();

    uint32_t get_mtu_size() const override { return 1500; }
    uint32_t get_align_preference() const override { return 0; }
    void get_ifname(char *name, uint8_t size) const override
    {
        if(size > 0) name[0] = 'e';
        if(size > 1) name[1] = 'n';
        if(size > 2) name[2] = '\0';
    }
    uint8_t get_hwaddr_size() const override { return 6; }
    bool get_hwaddr(uint8_t *addr) const override;
    void set_hwaddr(const uint8_t *addr) override;

    bool link_out(emac_mem_buf_t *buf) override;
    bool power_up() override;
    void power_down() override;

    void set_link_input_cb(emac_link_input_cb_t input_cb) override
    {
        _emac_input_data_cb = input_cb;
    }
    void set_link_state_cb(emac_link_state_change_cb_t state_cb) override
    {
        _emac_link_state_cb = state_cb;
        if(_link_state && _emac_link_state_cb)
        {
            _emac_link_state_cb(true);
        }
    }
    void add_multicast_group(const uint8_t *address) override { (void)address; }
    void remove_multicast_group(const uint8_t *address) override { (void)address; }
    void set_all_multicast(bool all) override { (void)all; }
    void set_memory_manager(EMACMemoryManager &mem_mngr) override
    {
        _memory_manager = &mem_mngr;
    }

private:
    static void ether_callback(ether_callback_args_t *p_args);
    void rx_thread();
    void phy_poll();
    void configure_pins();

public:
    /* FSP Ethernet instances (public: referenced by static cfg structs in .cpp) */
    static ether_instance_ctrl_t s_ether_ctrl;
    static ether_phy_instance_ctrl_t s_phy_ctrl;
    static ether_instance_descriptor_t s_rx_descriptors[4];
    static ether_instance_descriptor_t s_tx_descriptors[4];
    static uint8_t s_ether_buffers[8][1536];
    static uint8_t *s_pp_ether_buffers[8];
    static void ether_isr_cb(ether_callback_args_t *p_args) { ether_callback(p_args); }

private:
    EMACMemoryManager *_memory_manager;
    emac_link_input_cb_t _emac_input_data_cb;
    emac_link_state_change_cb_t _emac_link_state_cb;
    bool _link_state;
    bool _powered_up;
    uint8_t _mac_addr[6];
    rtos::Thread *_rx_thread;
    rtos::Semaphore _rx_sem;
};
