#pragma once

#include "eth_plugin_interface.h"
#include <string.h>

// Number of selectors defined in this plugin. Should match the enum `selector_t`.
// EDIT THIS: Put in the number of selectors your plugin is going to support.
#define NUM_SELECTORS 6

// Name of the plugin.
// EDIT THIS: Replace with your plugin name.
#define PLUGIN_NAME "Alkemi"

// Enumeration of the different selectors possible.
// Should follow the exact same order as the array declared in main.c
// EDIT THIS: Change the naming (`selector_t`), and add your selector names.
typedef enum {
    // SWAP_EXACT_ETH_FOR_TOKENS = 0,
    // BOILERPLATE_DUMMY_2,
    ALKEMI_WITHDRAW,
    ALKEMI_REPAY_BORROW,
    ALKEMI_SUPPLY,
    ALKEMI_BORROW,
    ALKEMI_LIQUIDATE_BORROW,
    ALKEMI_CLAIM_ALK,
} alkemiSelector_t;

// Enumeration used to parse the smart contract data.
// EDIT THIS: Adapt the parameter names here.
typedef enum {
    AMOUNT = 0,
    ASSET,
    TARGET_ACCOUNT,
    HOLDER,
    ASSET_BORROW,
    ASSET_COLLATERAL,
    REQUESTED_AMOUNT_CLOSE,
    UNEXPECTED_PARAMETER
} parameter;

// EDIT THIS: Rename `BOILERPLATE` to be the same as the one initialized in `main.c`.
extern const uint8_t *const ALKEMI_SELECTORS[NUM_SELECTORS];

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
// EDIT THIS: This struct is used by your plugin to save the parameters you parse. You
// will need to adapt this struct to your plugin.
typedef struct context_t {
    // For display.
    uint8_t amount[INT256_LENGTH];
    uint8_t holder[ADDRESS_LENGTH];
    uint8_t asset[ADDRESS_LENGTH];
    uint8_t assetCollateral[ADDRESS_LENGTH];
    char ticker[MAX_TICKER_LEN];
    char ticker2[MAX_TICKER_LEN];
    uint8_t decimals;
    uint8_t token_found;
    uint8_t token2_found;

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.
    uint16_t offset;     // Offset at which the array or struct starts.

    // For both parsing and display.
    alkemiSelector_t selectorIndex;
} context_t;

_Static_assert(sizeof(context_t) <= 5 * 32, "Shared context is too big");

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");
