#pragma once

namespace lost
{

// defines a range within a string that describes a physical line
// same semantics as stdlib iterators
struct Range
{
  Range() : begin(0), end(0) {}
  Range(uint32_t v1, uint32_t v2) : begin(v1), end(v2) {}
  uint32_t begin;
  uint32_t end;
};

}

