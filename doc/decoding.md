Generating translations using the trained model
===============================================

If we obtained the `best_dev_log_ppl.model.params` file in the output directory
by [training process](https://github.com/odashi/nmtkit/tree/master/doc/training_ja.md),
we can generate output sentences using `decode` command:

    src/bin/decode \
        --model model \
        < submodules/small_parallel_enja/test.en \
        > result.ja

Note that the input data of the `decode` should be tokenized in advance.

If the command failed due to out-of-memory, we can specify the amount of the
memory using `--dynet-mem` option as same as `train` command:

    src/bin/decode \
        --dynet-mem 4096 \
        --model model \
        < submodules/small_parallel_enja/test.en \
        > result.ja

If you want to generate HTML file with more detailed information during the
decoding process, use `--format html` option:

    src/bin/decode \
        --model model \
        --format html \
        < submodules/small_parallel_enja/test.en \
        > result.ja.html
