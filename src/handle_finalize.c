#include "alkemi_plugin.h"

void handle_finalize(ethPluginFinalize_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;

    msg->uiType = ETH_UI_TYPE_GENERIC;

    // EDIT THIS: Set the total number of screen you will need.
    // EDIT THIS: Handle this case like you wish to (i.e. maybe no additional screen needed?).
    // If the beneficiary is NOT the sender, we will need an additional screen to display it.
    // if (memcmp(msg->address, context->holder, ADDRESS_LENGTH) != 0) {
    //     msg->numScreens += 1;
    // }

    switch (context->selectorIndex) {
        case ALKEMI_CLAIM_ALK:
            msg->numScreens = 1;
            break;
        case ALKEMI_REPAY_BORROW:
        case ALKEMI_SUPPLY:
        case ALKEMI_WITHDRAW:
        case ALKEMI_BORROW:
            msg->numScreens = 2;
            break;
        case ALKEMI_LIQUIDATE_BORROW:
            msg->numScreens = 5;
            break;
        // Keep this
        default:
            msg->numScreens = 0;
    }

    // EDIT THIS: set `tokenLookup1` (and maybe `tokenLookup2`) to point to
    // token addresses you will info for (such as decimals, ticker...).
    msg->tokenLookup1 = context->asset;
    msg->tokenLookup2 = context->assetCollateral;

    msg->result = ETH_PLUGIN_RESULT_OK;
}
