#ifndef NMTKIT_BACKWARD_ENCODER_H_
#define NMTKIT_BACKWARD_ENCODER_H_

#include <boost/serialization/base_object.hpp>
#include <dynet/lstm.h>
#include <dynet/model.h>
#include <nmtkit/encoder.h>
#include <nmtkit/serialization_utils.h>

namespace nmtkit {

// Backward encoder.
class BackwardEncoder : public Encoder {
  BackwardEncoder(const BackwardEncoder &) = delete;
  BackwardEncoder(BackwardEncoder &&) = delete;
  BackwardEncoder & operator=(const BackwardEncoder &) = delete;
  BackwardEncoder & operator=(BackwardEncoder &&) = delete;

public:
  // Initializes an empty encoder object.
  BackwardEncoder() {}

  // Initializes an encoder object.
  // Arguments:
  //   num_layers: Depth of the RNN stacks.
  //   vocab_size: Vocabulary size of the input sequences.
  //   embed_size: Number of units in the embedding layer.
  //   hidden_size: Number of units in each RNN hidden layer.
  //   model: Model object for training.
  BackwardEncoder(
      unsigned num_layers,
      unsigned vocab_size,
      unsigned embed_size,
      unsigned hidden_size,
      dynet::Model * model);

  ~BackwardEncoder() override {}

  void build(
      const std::vector<std::vector<unsigned>> & input_ids,
      dynet::ComputationGraph * cg,
      std::vector<dynet::expr::Expression> * output_states,
      dynet::expr::Expression * final_state) override;

  unsigned getStateSize() const { return hidden_size_; }
  unsigned getFinalStateSize() const { return hidden_size_; }

private:
  // Boost serialization interface.
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive & ar, const unsigned) {
    ar & boost::serialization::base_object<Encoder>(*this);
    ar & num_layers_;
    ar & vocab_size_;
    ar & embed_size_;
    ar & hidden_size_;
    ar & rnn_;
    ar & p_lookup_;
  }

  unsigned num_layers_;
  unsigned vocab_size_;
  unsigned embed_size_;
  unsigned hidden_size_;
  dynet::LSTMBuilder rnn_;
  dynet::LookupParameter p_lookup_;
};

}  // namespace nmtkit

NMTKIT_SERIALIZATION_DECL(nmtkit::BackwardEncoder);

#endif  // NMTKIT_BACKWARD_ENCODER_H_