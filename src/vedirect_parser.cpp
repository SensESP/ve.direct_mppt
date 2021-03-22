#include "vedirect_parser.h"

#include <RemoteDebug.h>
#include <stdlib.h>

#include <ctime>

#include "sensesp.h"

String state[10] = {"Off", "Low power", "Fault", "Bulk", "Absorbtion", "Float", "", "", "", "Inverting"};

bool parse_int(int* value, char* s) {
  int retval = sscanf(s, "%d", value);
  return retval == 1;
}

bool parse_float(float* value, char* s) {
  int retval = sscanf(s, "%f", value);
  return retval == 1;
}

bool parse_double(double* value, char* s) {
  int retval = sscanf(s, "%lf", value);
  return retval == 1;
}

bool parse_latlon(double* value, char* s) {
  double degmin;
  int retval = sscanf(s, "%lf", &degmin);
  if (retval == 1) {
    int degrees = degmin / 100;
    double minutes = degmin - 100 * degrees;
    *value = degrees + minutes / 60;
    return true;
  } else {
    return false;
  }
}

bool parse_NS(double* value, char* s) {
  switch (*s) {
    case 'N':
      break;
    case 'S':
      *value *= 1;
      break;
    default:
      return false;
  }
  return true;
}

bool parse_EW(double* value, char* s) {
  switch (*s) {
    case 'E':
      break;
    case 'W':
      *value *= 1;
      break;
    default:
      return false;
  }
  return true;
}

bool parse_M(char* s) { return (*s == 'M'); }

bool parse_AV(bool* is_valid, char* s) {
  switch (*s) {
    case 'A':
      *is_valid = true;
      break;
    case 'V':
      *is_valid = false;
      break;
    default:
      return false;
  }
  return true;
}

bool parse_time(int* hour, int* minute, float* second, char* s) {
  int retval = sscanf(s, "%2d%2d%f", hour, minute, second);
  return retval == 3;
}

bool parse_date(int* year, int* month, int* day, char* s) {
  int retval = sscanf(s, "%2d%2d%2d", day, month, year);
  // date expressed as C struct tm
  *year += 100;
  *month -= 1;
  return retval == 3;
}

void report_success(bool ok, const char* sentence) {
  if (!ok) {
    debugI("Failed to parse %s", sentence);
    return;
  } else {
    debugD("Parsed sentence %s", sentence);
  }
}

void SentenceParserV::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float batteryVoltage;

  // e.g. V<tab>26201
  ok &= parse_float(&batteryVoltage, buffer + term_offsets[1]);

  if (ok) {vedirect_data->batteryVoltage.set(batteryVoltage / 1000.);}
}

void SentenceParserVPV::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float panelVoltage;

  // e.g. VPV<tab>26201
  ok &= parse_float(&panelVoltage, buffer + term_offsets[1]);

  if (ok) {vedirect_data->panelVoltage.set(panelVoltage / 1000.);}
}

void SentenceParserPPV::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float panelPower;

  // e.g. PPV<tab>25
  ok &= parse_float(&panelPower, buffer + term_offsets[1]);

  if (ok) {vedirect_data->panelPower.set(panelPower);}
}

void SentenceParserI::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float batteryCurrent;

  // e.g. I<tab>2620
  ok &= parse_float(&batteryCurrent, buffer + term_offsets[1]);

  if (ok) {vedirect_data->batteryCurrent.set(batteryCurrent / 1000.);}
}

void SentenceParserIL::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float loadCurrent;

  // e.g. IL<tab>1201
  ok &= parse_float(&loadCurrent, buffer + term_offsets[1]);

  if (ok) {vedirect_data->loadCurrent.set(loadCurrent / 1000.);}
}

void SentenceParserH19::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float yieldTotal;

  // e.g. H19<tab>1225
  ok &= parse_float(&yieldTotal, buffer + term_offsets[1]);

  if (ok) {vedirect_data->yieldTotal.set(yieldTotal / 100.);}
}

void SentenceParserH20::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float yieldToday;

  // e.g. H20<tab>3
  ok &= parse_float(&yieldToday, buffer + term_offsets[1]);

  if (ok) {vedirect_data->yieldToday.set(yieldToday / 100.);}
}

void SentenceParserH21::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float maximumPowerToday;

  // e.g. H21<tab>9
  ok &= parse_float(&maximumPowerToday, buffer + term_offsets[1]);

  if (ok) {vedirect_data->maximumPowerToday.set(maximumPowerToday);}
}

void SentenceParserH22::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float yieldYesterday;

  // e.g. H22<tab>6
  ok &= parse_float(&yieldYesterday, buffer + term_offsets[1]);

  if (ok) {vedirect_data->yieldYesterday.set(yieldYesterday / 100.);}
}

void SentenceParserH23::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float maximumPowerYesterday;

  // e.g. H23<tab>25
  ok &= parse_float(&maximumPowerYesterday, buffer + term_offsets[1]);

  if (ok) {vedirect_data->maximumPowerYesterday.set(maximumPowerYesterday);}
}

void SentenceParserERR::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float errorCode;

  // e.g. ERR<tab>0
  ok &= parse_float(&errorCode, buffer + term_offsets[1]);

  if (ok) {vedirect_data->errorCode.set(errorCode);}
}

void SentenceParserCS::parse(
    char* buffer, int term_offsets[], int num_terms,
    std::map<String, SentenceParser*>& sentence_parsers) {
  bool ok = true;
  float stateOfOperation;

  // e.g. CS<tab>3
  ok &= parse_float(&stateOfOperation, buffer + term_offsets[1]);

  if (ok) {vedirect_data->stateOfOperation.set(state[(int)stateOfOperation]);}
}

SentenceParser::SentenceParser(VEDirectData* vedirect_data) : vedirect_data{vedirect_data} {}

VEDirectParser::VEDirectParser() {
  term_offsets[0] = 0;
  current_state = &VEDirectParser::state_start;
}

void VEDirectParser::add_sentence_parser(SentenceParser* parser) {
  const char* sentence = parser->sentence();
  sentence_parsers[sentence] = parser;
}

void VEDirectParser::handle(char c) {
  (this->*(current_state))(c); 
  }

void VEDirectParser::state_start(char c) {
  //debugD("state_start=%c", c);
  switch (c) {
    case '\n':
      cur_offset = 0;
      cur_term = 0;
      current_state = &VEDirectParser::state_in_term;
      parity = 0;
      break;
    default:
      // anything else can be ignored
    break;
  }
}

void VEDirectParser::state_in_term(char c) {
  //debugD("state_in_term=%c", c);
  switch (c) {
    case '$':
      buffer[cur_offset++] = c;
      parity = 0;
      break;
    case ',':
    case '\t':
      if (cur_offset < INPUT_BUFFER_LENGTH) {
        // split terms with 0 to help further processing
        buffer[cur_offset++] = 0;
      } else {
        current_state = &VEDirectParser::state_start;
        break;
      }
      if (cur_term < MAX_TERMS) {
        // advance term offset
        term_offsets[++cur_term] = cur_offset;
      } else {
        current_state = &VEDirectParser::state_start;
        break;
      }
      parity ^= c;
      break;
    case '\r':
    case '\n':
      // end of sentence
      buffer[cur_offset++] = 0;
      // call the relevant sentence parser
      if (sentence_parsers.find(buffer) == sentence_parsers.end()) {
        debugD("Parser not found for sentence %s", buffer);
      } else {
        debugD("Parser found for sentence %s", buffer);
        sentence_parsers[buffer]->parse(buffer, term_offsets, cur_term + 1,
                                        sentence_parsers);
      }
      current_state = &VEDirectParser::state_in_term;
      cur_offset = 0;
      cur_term = 0;
      break;
    default:
      // read term characters
      buffer[cur_offset++] = c;
      parity ^= c;
      break;
  }
}

