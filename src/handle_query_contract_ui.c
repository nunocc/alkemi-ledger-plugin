#include <stdbool.h>
#include "alkemi_plugin.h"

// EDIT THIS: You need to adapt / remove the static functions (set_send_ui, set_receive_ui ...) to
// match what you wish to display.

bool set_asset_address_ui(ethQueryContractUI_t *msg, context_t *context) {
    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;
    bool ret = false;

    switch (context->selectorIndex) {
        case ALKEMI_WITHDRAW:
        case ALKEMI_SUPPLY:
        case ALKEMI_BORROW:
        case ALKEMI_REPAY_BORROW:
        case ALKEMI_LIQUIDATE_BORROW:
            // Prefix the address with `0x`.
            msg->msg[0] = '0';
            msg->msg[1] = 'x';
            ret = getEthAddressStringFromBinary(context->asset,
                                                msg->msg + 2,
                                                chainid);
            break;
        default:
            break;
    }
    return ret;
}

bool set_holder_address_ui(ethQueryContractUI_t *msg, context_t *context) {
    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;
    bool ret = false;

    switch (context->selectorIndex) {
        case ALKEMI_CLAIM_ALK:
        case ALKEMI_LIQUIDATE_BORROW:
            // Prefix the address with `0x`.
            msg->msg[0] = '0';
            msg->msg[1] = 'x';
            ret = getEthAddressStringFromBinary(context->holder,
                                                msg->msg + 2,
                                                chainid);
            break;
        default:
            break;
    }
    return ret;
}

bool set_address_collateral_ui(ethQueryContractUI_t *msg, context_t *context) {
    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;
    bool ret = false;

    switch (context->selectorIndex) {
        case ALKEMI_LIQUIDATE_BORROW:
            // Prefix the address with `0x`.
            msg->msg[0] = '0';
            msg->msg[1] = 'x';
            ret = getEthAddressStringFromBinary(context->assetCollateral,
                                                msg->msg + 2,
                                                chainid);
            break;
        default:
            break;
    }
    return ret;
}

static bool set_third_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;

    switch (context->selectorIndex) {
        case ALKEMI_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Asset Address.", msg->titleLength);
            ret = set_asset_address_ui(msg, context);
            break;
        default:
            break;
    }
    return ret;
}

static bool set_fourth_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;

    switch (context->selectorIndex) {
        case ALKEMI_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Collateral Asset.", msg->titleLength);
            strlcpy(msg->msg, context->ticker2, msg->msgLength);
            ret = true;
            break;
        default:
            break;
    }
    return ret;
}

static bool set_fifth_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;

    switch (context->selectorIndex) {
        case ALKEMI_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Coll. Asset Addr.", msg->titleLength);
            ret = set_address_collateral_ui(msg, context);
            break;
        default:
            break;
    }
    return ret;
}

static bool is_max_amount(const uint8_t *buffer, uint32_t buffer_size) {
    for (uint32_t i = 0; i < buffer_size; ++i) {
        if (buffer[i] != UINT8_MAX) {
            return false;
        }
    }
    return true;
}

static bool set_second_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;

    switch (context->selectorIndex) {
        case ALKEMI_SUPPLY:
        case ALKEMI_WITHDRAW:
        case ALKEMI_BORROW:
        case ALKEMI_REPAY_BORROW:
            strlcpy(msg->title, "Asset Address.", msg->titleLength);
            ret = set_asset_address_ui(msg, context);
            break;
        case ALKEMI_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Amount.", msg->titleLength);
            if (is_max_amount(context->amount, sizeof(context->amount))) {
                strlcpy(msg->msg, context->ticker, msg->msgLength);
                strlcat(msg->msg, " Max", msg->msgLength);
                ret = true;
            } else {
                ret = amountToString(context->amount,
                                     sizeof(context->amount),
                                     context->decimals,
                                     context->ticker,
                                     msg->msg,
                                     msg->msgLength);
            }
            break;
        default:
            break;
    }
    return ret;
}

static bool set_first_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;

    switch (context->selectorIndex) {
        case ALKEMI_WITHDRAW:
        case ALKEMI_SUPPLY:
        case ALKEMI_BORROW:
        case ALKEMI_REPAY_BORROW:
            strlcpy(msg->title, "Amount.", msg->titleLength);
            if (is_max_amount(context->amount, sizeof(context->amount))) {
                strlcpy(msg->msg, context->ticker, msg->msgLength);
                strncat(msg->msg, " Max", msg->msgLength);
                ret = true;
            } else {
                ret = amountToString(context->amount,
                                     sizeof(context->amount),
                                     context->decimals,
                                     context->ticker,
                                     msg->msg,
                                     msg->msgLength);
            }
            break;
        case ALKEMI_CLAIM_ALK:
        case ALKEMI_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Target Account.", msg->titleLength);
            ret = set_holder_address_ui(msg, context);
            break;
    }
    return ret;
}

void handle_query_contract_ui(ethQueryContractUI_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    bool ret = false;

    // Ensure all the parameters have been received. When it is the
    // case, next_param is always set to "UNEXPECTED_PARAMETER".
    if (context->next_param != UNEXPECTED_PARAMETER) {
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    // EDIT THIS: Adapt the cases for the screens you'd like to display.
    switch (msg->screenIndex) {
        case 0:
            ret = set_first_param_ui(msg, context);
            break;
        case 1:
            ret = set_second_param_ui(msg, context);
            break;
        case 2:
            ret = set_third_param_ui(msg, context);
            break;
        case 3:
            ret = set_fourth_param_ui(msg, context);
            break;
        case 4:
            ret = set_fifth_param_ui(msg, context);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
