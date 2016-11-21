#include "config.h"

#include <nmtkit/character_vocabulary.h>

#include <fstream>
#include <functional>
#include <boost/algorithm/string.hpp>
#include <nmtkit/array.h>
#include <nmtkit/corpus.h>
#include <nmtkit/exception.h>

using namespace std;

namespace {

// Check whether the character is a UTF-8 first byte or not.
//
// Arguments:
//   c: Target character.
//
// Returns:
//   true if `c` is a UTF-8 first byte, false otherwise.
bool isUTF8FirstByte(char c) {
  return (c & 0x80) == 0 || (c & 0xc0) == 0xc0;
}

// Separates UTF-8 string into its letters.
//
// Arguments:
//   str: Target string.
//
// Returns:
//   A list of UTF-8 letters.
vector<string> convertToLetters(const string & str) {
  const unsigned len = str.size();
  unsigned prev = 0;
  vector<string> letters;
  while (prev < len) {
    unsigned next = prev + 1;
    while (next < len && !isUTF8FirstByte(str[next])) ++next;
    letters.emplace_back(str.substr(prev, next - prev));
    prev = next;
  }
  return letters;
}

}  // namespace

namespace nmtkit {

CharacterVocabulary::CharacterVocabulary(
    const string & corpus_filename,
    unsigned size) {
  NMTKIT_CHECK(size >= 3, "Size should be equal or greater than 3.");
  ifstream ifs(corpus_filename);
  NMTKIT_CHECK(
      ifs.is_open(),
      "Could not open corpus file to load: " + corpus_filename);

  // Counts letter frequencies.
  map<string, unsigned> freq;
  string line;
  while (Corpus::readLine(&ifs, &line)) {
    for (const string & letter : ::convertToLetters(line)) {
      ++freq[letter];
    }
  }

  // Selects most frequent letters.
  vector<pair<unsigned, string>> entries;
  for (const auto & entry : freq) {
    entries.emplace_back(make_pair(entry.second, entry.first));
  }
  Array::sort(&entries, greater<pair<unsigned, string>>());
  
  // Store entries.
  stoi_["<unk>"] = 0;
  stoi_["<s>"] = 1;
  stoi_["</s>"] = 2;
  itos_.emplace_back("<unk>");
  itos_.emplace_back("<s>");
  itos_.emplace_back("</s>");
  for (unsigned i = 3; i < size && i - 3 < entries.size(); ++i) {
    const string & letter = entries[i - 3].second;
    stoi_[letter] = i;
    itos_.emplace_back(letter);
  }
}

unsigned CharacterVocabulary::getID(const string & word) const {
  const auto &entry = stoi_.find(word);
  if (entry == stoi_.end()) return 0;  // ID of <unk>
  return entry->second;
}

string CharacterVocabulary::getWord(unsigned id) const {
  if (id >= itos_.size()) return "<unk>";  // out of range
  return itos_[id];
}

vector<unsigned> CharacterVocabulary::convertToIDs(
    const string & sentence) const {
  vector<unsigned> ids;
  for (const string & letter : ::convertToLetters(sentence)) {
    ids.emplace_back(getID(letter));
  }
  return ids;
}

string CharacterVocabulary::convertToSentence(
    const vector<unsigned> & word_ids) const {
  vector<string> letters;
  for (const unsigned word_id : word_ids) {
    letters.emplace_back(getWord(word_id));
  }
  return boost::join(letters, "");
}

unsigned CharacterVocabulary::size() const {
  return itos_.size();
}

}  // namespace nmtkit

NMTKIT_SERIALIZATION_IMPL(nmtkit::CharacterVocabulary);