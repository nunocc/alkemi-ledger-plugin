#include <stdint.h>
#include "alkemi_plugin.h"

// List of selectors supported by this plugin.
// EDIT THIS: Adapt the variable names and change the `0x` values to match your selectors.

/* From contract https://etherscan.io/address/0x82c19bdd07f9ca911ab8bc7bd5faf092736cfa64#code
from proxy https://etherscan.io/address/0x397c315d64D74d82A731d656f9C4D586D200F90A#code */
static const uint8_t ALKEMI_WITHDRAW_SELECTOR[SELECTOR_SIZE] = {0xf3,
                                                                0xfe,
                                                                0xf3,
                                                                0xa3};  // 0xf3fef3a3
static const uint8_t ALKEMI_REPAY_BORROW_SELECTOR[SELECTOR_SIZE] = {0xab,
                                                                    0xdb,
                                                                    0x5e,
                                                                    0xa8};  // 0xabdb5ea8
static const uint8_t ALKEMI_SUPPLY_SELECTOR[SELECTOR_SIZE] = {0xf2,
                                                              0xb9,
                                                              0xfd,
                                                              0xb8};  // 0xf2b9fdb8
static const uint8_t ALKEMI_BORROW_SELECTOR[SELECTOR_SIZE] = {0x4b,
                                                              0x8a,
                                                              0x35,
                                                              0x29};  // 0x4b8a3529
static const uint8_t ALKEMI_LIQUIDATE_BORROW_SELECTOR[SELECTOR_SIZE] = {0xe6,
                                                                        0x16,
                                                                        0x04,
                                                                        0xcf};  // 0xe61604cf

/* From contract https://etherscan.io/address/0x14716c982fd8b7f1e8f0b4dbb496dce438a29d93 */
static const uint8_t ALKEMI_CLAIM_ALK_SELECTOR[SELECTOR_SIZE] = {0x71,
                                                                 0xd6,
                                                                 0xe8,
                                                                 0x92};  // 0x71d6e892

// Array of all the different boilerplate selectors. Make sure this follows the same order as the
// enum defined in `boilerplate_plugin.h`
// EDIT THIS: Use the names of the array declared above.
const uint8_t *const ALKEMI_SELECTORS[NUM_SELECTORS] = {ALKEMI_WITHDRAW_SELECTOR,
                                                        ALKEMI_REPAY_BORROW_SELECTOR,
                                                        ALKEMI_SUPPLY_SELECTOR,
                                                        ALKEMI_BORROW_SELECTOR,
                                                        ALKEMI_LIQUIDATE_BORROW_SELECTOR,
                                                        ALKEMI_CLAIM_ALK_SELECTOR};
