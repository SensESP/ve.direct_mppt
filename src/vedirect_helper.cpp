#include <Arduino.h>
#include "vedirect_helper.h"

#include "signalk/signalk_output.h"

VEDirectInput::VEDirectInput(Stream* rx_stream)
    : Sensor() {
  rx_stream_ = rx_stream;

  vedirect_parser_.add_sentence_parser(new SentenceParserV(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserVPV(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserPPV(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserI(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserIL(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserH19(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserH20(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserH21(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserH22(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserH23(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserERR(&vedirect_data_));
  vedirect_parser_.add_sentence_parser(new SentenceParserCS(&vedirect_data_));

}

void VEDirectInput::enable() {
  // enable reading the serial port
  app.onAvailable(*rx_stream_, [this]() {
    while (rx_stream_->available()) {
      vedirect_parser_.handle(rx_stream_->read());
    }
  });
}

//SKMetadata* metadata = new SKMetadata("W", "Panel Power", "Panel Power", "Pnl Pow");

VEDirectInput* setup_vedirect(Stream* rx_stream) {
  VEDirectInput* victronDevice = new VEDirectInput(rx_stream);
  victronDevice->vedirect_data_.batteryVoltage.connect_to(
      new SKOutputNumber("electrical.batteries.mainBattery.voltage", "/ve.direct/batteryVoltagePath"));
  victronDevice->vedirect_data_.panelVoltage.connect_to(
      new SKOutputNumber("electrical.solar.panel.voltage", "/ve.direct/panelVoltagePath"));
  victronDevice->vedirect_data_.panelPower.connect_to(
      new SKOutputNumber("electrical.solar.panel.power", "/ve.direct/panelPowerPath", 
        new SKMetadata("W", "Panel Power", "Panel Power", "Pnl Pow")));
  victronDevice->vedirect_data_.batteryCurrent.connect_to(
      new SKOutputNumber("electrical.batteries.mainBattery.current", "/ve.direct/chargeCurrentPath"));
  victronDevice->vedirect_data_.loadCurrent.connect_to(
      new SKOutputNumber("electrical.batteries.loadCurrent.current", "/ve.direct/loadCurrentPath"));
  victronDevice->vedirect_data_.yieldTotal.connect_to(
      new SKOutputNumber("electrical.solar.yieldTotal", "/ve.direct/yieldTotalPath", 
        new SKMetadata("kWh", "Yield Total", "Yield Total", "Yield Total")));
  victronDevice->vedirect_data_.yieldToday.connect_to(
      new SKOutputNumber("electrical.solar.charger1.yieldToday", "/ve.direct/yieldTodayPath", 
        new SKMetadata("kWh", "Yield Today", "Yield Today", "Yield Today")));
  victronDevice->vedirect_data_.maximumPowerToday.connect_to(
      new SKOutputNumber("electrical.solar.charger1.maximumPowerToday", "/ve.direct/maximumPowerTodayPath", 
        new SKMetadata("W", "Maximum Power Today", "Maximum Power Today", "Max Power Today")));
  victronDevice->vedirect_data_.yieldYesterday.connect_to(
      new SKOutputNumber("electrical.solar.charger1.yieldYesterday", "/ve.direct/yieldYesterdayPath", 
        new SKMetadata("kWh", "Yield Yesterday", "Yield Yesterday", "Yield Yesterday")));
  victronDevice->vedirect_data_.maximumPowerYesterday.connect_to(
      new SKOutputNumber("electrical.solar.charger1.maximumPowerYesterday", "/ve.direct/maximumPowerYesterdayPath", 
        new SKMetadata("W", "Maximum Power Yesterday", "Maximum Power Yesterday", "Max Power Yesterday")));
  victronDevice->vedirect_data_.errorCode.connect_to(
      new SKOutputNumber("electrical.solar.charger1.errorCode", "/ve.direct/errorCodePath"));
  victronDevice->vedirect_data_.stateOfOperation.connect_to(
      new SKOutputString("electrical.solar.charger1.stateOfOperation", "/ve.direct/stateOfOperationPath"));

  return victronDevice;
}

