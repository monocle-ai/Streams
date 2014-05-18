#ifndef GENERATED_STREAM_PROVIDER
#define GENERATED_STREAM_PROVIDER

#include "StreamProvider.h"

#include "Utility.h"

template<typename T, typename Generator>
class GeneratedStreamProvider : public StreamProvider<T> {

public:
    GeneratedStreamProvider(Generator&& generator)
        : generator_(generator) {}

    std::shared_ptr<T> get() override {
        return current_;
    }

    bool advance() override {
        current_ = std::make_shared<T>(generator_());
        return true;
    }

    std::pair<int, int> print(std::ostream& os, int indent) const override {
        this->print_indent_arrow(os, indent);
        os << "[generated stream]\n";
        return {0, 1};
    }

private:
    Generator generator_;
    std::shared_ptr<T> current_;

};


#endif
