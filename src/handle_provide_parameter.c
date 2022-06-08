#include "alkemi_plugin.h"

// EDIT THIS: Remove this function and write your own handlers!
// One param functions handler
static void handle_multiple(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case ASSET:
            copy_address(context->asset, msg->parameter, sizeof(context->asset));
            context->next_param = AMOUNT;
            break;
        case AMOUNT:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case HOLDER:  // claimALK
            copy_address(context->holder, msg->parameter, sizeof(context->holder));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_liquidate_borrow(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case TARGET_ACCOUNT:  // LiquidateBorrow
            copy_address(context->holder, msg->parameter, sizeof(context->holder));
            context->next_param = ASSET_BORROW;
            break;
        case ASSET_BORROW:  // LiquidateBorrow
            copy_address(context->asset, msg->parameter, sizeof(context->asset));
            context->next_param = ASSET_COLLATERAL;
            break;
        case ASSET_COLLATERAL:
            copy_address(context->assetCollateral,
                         msg->parameter,
                         sizeof(context->assetCollateral));
            context->next_param = REQUESTED_AMOUNT_CLOSE;
            break;
        case REQUESTED_AMOUNT_CLOSE:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: adapt the cases and the names of the functions.
    switch (context->selectorIndex) {
        case ALKEMI_WITHDRAW:
        case ALKEMI_REPAY_BORROW:
        case ALKEMI_SUPPLY:
        case ALKEMI_BORROW:
        case ALKEMI_CLAIM_ALK:
            handle_multiple(msg, context);
            break;
        case ALKEMI_LIQUIDATE_BORROW:
            handle_liquidate_borrow(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}