#include "config.h"

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <vector>
#include <string>
#include <nmtkit/vocabulary.h>

using namespace std;

BOOST_AUTO_TEST_SUITE(VocabularyTest)

BOOST_AUTO_TEST_CASE(CheckLoadFromVocabularyFile_En) {
  nmtkit::Vocabulary vocab("data/small.en.vocab");
  BOOST_CHECK_EQUAL(500, vocab.size());
  const vector<string> topk {
    "<unk>", "<s>", "</s>", ".", "the", "to", "i", "you"};
  for (unsigned i = 0; i < topk.size(); ++i) {
    BOOST_CHECK_EQUAL(i, vocab.getID(topk[i]));
    BOOST_CHECK_EQUAL(topk[i], vocab.getWord(i));
  }
  BOOST_CHECK_EQUAL(0, vocab.getID("unknown-word"));
  BOOST_CHECK_EQUAL("<unk>", vocab.getWord(500));
  BOOST_CHECK_EQUAL("<unk>", vocab.getWord(-1));
}

BOOST_AUTO_TEST_CASE(CheckLoadFromVocabularyFile_Ja) {
  nmtkit::Vocabulary vocab("data/small.ja.vocab");
  BOOST_CHECK_EQUAL(500, vocab.size());
  const vector<string> topk {
    "<unk>", "<s>", "</s>", "。", "は", "い", "に", "を"};
  for (unsigned i = 0; i < topk.size(); ++i) {
    BOOST_CHECK_EQUAL(i, vocab.getID(topk[i]));
    BOOST_CHECK_EQUAL(topk[i], vocab.getWord(i));
  }
  BOOST_CHECK_EQUAL(0, vocab.getID("未知語"));
  BOOST_CHECK_EQUAL("<unk>", vocab.getWord(500));
  BOOST_CHECK_EQUAL("<unk>", vocab.getWord(-1));
}

BOOST_AUTO_TEST_CASE(CheckLoadFromCorpus_En) {
  nmtkit::Vocabulary vocab("data/small.en.tok", 100);
  BOOST_CHECK_EQUAL(100, vocab.size());
  const vector<string> topk {
    "<unk>", "<s>", "</s>", ".", "the", "to", "i", "you"};
  for (unsigned i = 0; i < topk.size(); ++i) {
    BOOST_CHECK_EQUAL(i, vocab.getID(topk[i]));
    BOOST_CHECK_EQUAL(topk[i], vocab.getWord(i));
  }
  BOOST_CHECK_EQUAL(0, vocab.getID("unknown-word"));
  BOOST_CHECK_EQUAL("<unk>", vocab.getWord(100));
  BOOST_CHECK_EQUAL("<unk>", vocab.getWord(-1));
}

BOOST_AUTO_TEST_CASE(CheckLoadFromCorpus_Ja) {
  nmtkit::Vocabulary vocab("data/small.ja.tok", 100);
  BOOST_CHECK_EQUAL(100, vocab.size());
  const vector<string> topk {
    "<unk>", "<s>", "</s>", "。", "は", "い", "に", "を"};
  for (unsigned i = 0; i < topk.size(); ++i) {
    BOOST_CHECK_EQUAL(i, vocab.getID(topk[i]));
    BOOST_CHECK_EQUAL(topk[i], vocab.getWord(i));
  }
  BOOST_CHECK_EQUAL(0, vocab.getID("未知語"));
  BOOST_CHECK_EQUAL("<unk>", vocab.getWord(100));
  BOOST_CHECK_EQUAL("<unk>", vocab.getWord(-1));
}

BOOST_AUTO_TEST_CASE(CheckConversion) {
  nmtkit::Vocabulary vocab("data/small.en.vocab");
  const vector<string> sentences {
    "anything that can go wrong , will go wrong .",
    "there is always light behind the clouds .",
    "and yet it moves .",
    "これ は 日本 語 の テスト 文 で す 。",
  };
  const vector<vector<unsigned>> expected {
    {0, 20, 41, 45, 134, 31, 37, 45, 134, 3},
    {39, 9, 85, 0, 400, 4, 0, 3},
    {56, 183, 16, 0, 3},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  };

  for (unsigned i = 0; i < sentences.size(); ++i) {
    vector<unsigned> observed = vocab.convertToIDs(sentences[i]);
    BOOST_CHECK_EQUAL_COLLECTIONS(
        expected[i].begin(), expected[i].end(),
        observed.begin(), observed.end());
  }
}

BOOST_AUTO_TEST_SUITE_END()
