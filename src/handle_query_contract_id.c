#include "alkemi_plugin.h"

// Sets the first screen to display.
void handle_query_contract_id(ethQueryContractID_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    // msg->name will be the upper sentence displayed on the screen.
    // msg->version will be the lower sentence displayed on the screen.

    // For the first screen, display the plugin name.
    strlcpy(msg->name, "Alkemi", msg->nameLength);

    // EDIT THIS: Adapt the cases by modifying the strings you pass to `strlcpy`.
    switch (context->selectorIndex) {
        // case SWAP_EXACT_ETH_FOR_TOKENS:
        //     strlcpy(msg->version, "Swap", msg->versionLength);
        //     break;
        case ALKEMI_WITHDRAW:
            strlcpy(msg->version, "Withdraw", msg->versionLength);
            break;
        case ALKEMI_REPAY_BORROW:
            strlcpy(msg->version, "Repay Borrow", msg->versionLength);
            break;
        case ALKEMI_SUPPLY:
            strlcpy(msg->version, "Supply", msg->versionLength);
            break;
        case ALKEMI_BORROW:
            strlcpy(msg->version, "Borrow", msg->versionLength);
            break;
        case ALKEMI_LIQUIDATE_BORROW:
            strlcpy(msg->version, "Liquidate Borrow", msg->versionLength);
            break;
        case ALKEMI_CLAIM_ALK:
            strlcpy(msg->version, "Claim ALKs", msg->versionLength);
            break;
        // Keep this
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}
