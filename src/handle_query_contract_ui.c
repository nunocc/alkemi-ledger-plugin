#include "alkemi_plugin.h"

// EDIT THIS: You need to adapt / remove the static functions (set_send_ui, set_receive_ui ...) to
// match what you wish to display.

void set_asset_address_ui(ethQueryContractUI_t *msg, context_t *context) {
    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    switch (context->selectorIndex) {
        case ALKEMI_WITHDRAW:
        case ALKEMI_SUPPLY:
        case ALKEMI_BORROW:
        case ALKEMI_REPAY_BORROW:
        case ALKEMI_LIQUIDATE_BORROW:
            // Prefix the address with `0x`.
            msg->msg[0] = '0';
            msg->msg[1] = 'x';
            getEthAddressStringFromBinary(context->asset,
                                          msg->msg + 2,
                                          msg->pluginSharedRW->sha3,
                                          chainid);
            break;
        default:
            break;
    }
}

void set_holder_address_ui(ethQueryContractUI_t *msg, context_t *context) {
    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    switch (context->selectorIndex) {
        case ALKEMI_CLAIM_ALK:
        case ALKEMI_LIQUIDATE_BORROW:
            // Prefix the address with `0x`.
            msg->msg[0] = '0';
            msg->msg[1] = 'x';
            getEthAddressStringFromBinary(context->holder,
                                          msg->msg + 2,
                                          msg->pluginSharedRW->sha3,
                                          chainid);
            break;
        default:
            break;
    }
}

void set_address_collateral_ui(ethQueryContractUI_t *msg, context_t *context) {
    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    switch (context->selectorIndex) {
        case ALKEMI_LIQUIDATE_BORROW:
            // Prefix the address with `0x`.
            msg->msg[0] = '0';
            msg->msg[1] = 'x';
            getEthAddressStringFromBinary(context->assetCollateral,
                                          msg->msg + 2,
                                          msg->pluginSharedRW->sha3,
                                          chainid);
            break;
        default:
            break;
    }
}

static void set_third_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case ALKEMI_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Asset Address.", msg->titleLength);
            set_asset_address_ui(msg, context);
            break;
        default:
            break;
    }
}

static void set_fourth_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case ALKEMI_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Collateral Asset.", msg->titleLength);
            strlcpy(msg->msg, context->ticker2, msg->msgLength);
            break;
        default:
            break;
    }
}

static void set_fifth_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case ALKEMI_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Coll. Asset Addr.", msg->titleLength);
            set_address_collateral_ui(msg, context);
            break;
        default:
            break;
    }
}

static void set_second_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case ALKEMI_SUPPLY:
        case ALKEMI_WITHDRAW:
        case ALKEMI_BORROW:
        case ALKEMI_REPAY_BORROW:
            strlcpy(msg->title, "Asset Address.", msg->titleLength);
            set_asset_address_ui(msg, context);
            break;
        case ALKEMI_LIQUIDATE_BORROW:
            strlcpy(msg->title, "Amount.", msg->titleLength);
            amountToString(context->amount,
                           sizeof(context->amount),
                           context->decimals,
                           context->ticker,
                           msg->msg,
                           msg->msgLength);
            break;
        default:
            break;
    }
}

static bool is_max_amount(const uint8_t *buffer, uint32_t buffer_size) {
    for (uint32_t i = 0; i < buffer_size; ++i) {
        if (buffer[i] != UINT8_MAX) {
            return false;
        }
    }
    return true;
}

static void set_first_param_ui(ethQueryContractUI_t *msg, context_t *context) {
    switch (context->selectorIndex) {
        case ALKEMI_WITHDRAW:
        case ALKEMI_SUPPLY:
        case ALKEMI_BORROW:
        case ALKEMI_REPAY_BORROW:
            strlcpy(msg->title, "Amount.", msg->titleLength);
            if (is_max_amount(context->amount, sizeof(context->amount))) {
                strlcpy(msg->msg, context->ticker, msg->msgLength);
                strncat(msg->msg, " Max", msg->msgLength);
            } else {
                amountToString(context->amount,
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
            set_holder_address_ui(msg, context);
            break;
    }
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

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

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: Adapt the cases for the screens you'd like to display.
    switch (msg->screenIndex) {
        case 0:
            set_first_param_ui(msg, context);
            break;
        case 1:
            set_second_param_ui(msg, context);
            break;
        case 2:
            set_third_param_ui(msg, context);
            break;
        case 3:
            set_fourth_param_ui(msg, context);
            break;
        case 4:
            set_fifth_param_ui(msg, context);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
