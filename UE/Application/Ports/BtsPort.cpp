#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"
#include "Sms.hpp"

namespace ue
{


BtsPort::BtsPort(common::ILogger &logger, common::ITransport &transport, common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"),
      transport(transport),
      phoneNumber(phoneNumber)
{}

void BtsPort::start(IBtsEventsHandler &handler)
{
    transport.registerMessageCallback([this](BinaryMessage msg) {handleMessage(msg);});
    transport.registerDisconnectedCallback([this]() {handleDisconnected();});
    this->handler = &handler;
}

void BtsPort::stop()
{
    transport.registerMessageCallback(nullptr);
    transport.registerDisconnectedCallback(nullptr);
    handler = nullptr;
}

void BtsPort::handleDisconnected()
{
    handler->handleDisconnected();
}

void BtsPort::handleMessage(BinaryMessage msg)
{
    try
    {
        common::IncomingMessage reader{msg};
        auto msgId = reader.readMessageId();
        auto from = reader.readPhoneNumber();
        auto to = reader.readPhoneNumber();

        switch (msgId)
        {
        case common::MessageId::Sib:
        {
            auto btsId = reader.readBtsId();
            handler->handleSib(btsId);
            break;
        }
        case common::MessageId::AttachResponse:
        {
            bool accept = reader.readNumber<std::uint8_t>() != 0u;
            if (accept)
                handler->handleAttachAccept();
            else
                handler->handleAttachReject();
            break;
        }
        case common::MessageId::Sms:
        {
            std::string text = reader.readRemainingText();
            logger.logDebug("BtsPort: SmsReceived from: ", from);
            logger.logDebug("BtsPort: SmsReceived messeage ", text);
            handler->handleSms(from, text);
            break;
        }
        case common::MessageId::CallRequest:
        {
            handler->handleCallRequest(from);
            break;
        }
        case common::MessageId::CallAccepted:
        {
            logger.logInfo("BTS handleMessage: CallAccepted");
            handler->handleCallAccepted();
            break;
        }
        case common::MessageId::CallDropped:
        {
            logger.logInfo("BTS handleMessage: CallDropped");
            handler->handleCallFailure("User dropped a call.");
            break;
        }
        case common::MessageId::UnknownRecipient:
        {
            if (reader.readMessageId() == common::MessageId::Sms) {
                 logger.logInfo("BTS handleMessage: UnknownRecipient - SMS");
                handler->handleSmsUnknownRecipient();
                break;
            } else {
                 logger.logInfo("BTS handleMessage: UnknownRecipient - Call");
                handler->handleCallUnknownRecipient();
            }
            break;
        }
        case common::MessageId::CallTalk:
        {
            logger.logInfo("BTS CallTalk from: ",from);
            std::string&& text =  reader.readRemainingText();
            handler->handleCallTalk(text);
            break;
        }
        default:
            logger.logError("unknown message: ", msgId, ", from: ", from);
            
        }
    }
    catch (std::exception const& ex)
    {
        logger.logError("handleMessage error: ", ex.what());
    }
}

void BtsPort::sendAttachRequest(common::BtsId btsId)
{
    logger.logDebug("sendAttachRequest: ", btsId);
    common::OutgoingMessage msg{common::MessageId::AttachRequest,
                                phoneNumber,
                                common::PhoneNumber{}};
    msg.writeBtsId(btsId);
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallAccept(common::PhoneNumber toPhoneNumber)
{
    logger.logInfo("BtsPort::sendCallAccept: ", to_string(toPhoneNumber));
    common::OutgoingMessage msg{common::MessageId::CallAccepted,
                                phoneNumber,
                                toPhoneNumber};
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallDropped(common::PhoneNumber toPhoneNumber)
{
    logger.logInfo("BtsPort::sendCallReject: ", to_string(toPhoneNumber));
    common::OutgoingMessage msg{common::MessageId::CallDropped,
                                phoneNumber,
                                toPhoneNumber};
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendCallRequest(common::PhoneNumber to)
{
    if (to == phoneNumber) {
        logger.logError("sendCallRequest - same PhoneNumber");

        handler->handleCallFailure("You can't call yourself.");
        return;
    }
    logger.logInfo("sendCallRequest - PhoneNumber to: ", to);
    common::OutgoingMessage msg{common::MessageId::CallRequest,
                                phoneNumber,
                                to};
    transport.sendMessage(msg.getMessage());
}

void BtsPort::sendSms(Sms &sms)
{
    logger.logInfo("sms send from:",sms.from," to:",sms.to);
    common::OutgoingMessage smsSendMsg{common::MessageId::Sms, sms.from, sms.to};
    smsSendMsg.writeText(sms.text);
    transport.sendMessage(smsSendMsg.getMessage());
}

void BtsPort::sendCallTalk(std::string& text, common::PhoneNumber to)
{
    logger.logInfo("Talk text: ",text,", to: ",to);
    common::OutgoingMessage msg{common::MessageId::CallTalk,phoneNumber, to};
    msg.writeText(text);
    transport.sendMessage(msg.getMessage());
}

}
