#ifndef DROP_WHILE_STREAM_PROVIDER_H
#define DROP_WHILE_STREAM_PROVIDER_H

#include "StreamProvider.h"

template<typename T, typename Predicate>
class DropWhileStreamProvider : public StreamProvider<T> {

public:
    DropWhileStreamProvider(StreamProviderPtr<T> source, Predicate&& predicate)
        : source_(std::move(source)), predicate_(predicate) {}

    std::shared_ptr<T> get() override {
        return current_;
    }

    bool advance() override {
        if(!dropped_) {
            dropped_ = true;
            while(source_->advance()) {
                current_ = source_->get();
                if(!predicate_(*current_)) {
                    return true;
                }
            }
            current_.reset();
            return false;
        }
        if(source_->advance()) {
            current_ = source_->get();
            return true;
        }
        current_.reset();
        return false;
    }

    std::pair<int, int> print(std::ostream& os, int indent) const override {
        this->print_indent_arrow(os, indent);
        os << "DropWhile:\n";
        auto sub = source_->print(os, indent + 1);
        return {sub.first + 1, sub.second};
    }

private:
    StreamProviderPtr<T> source_;
    Predicate predicate_;
    std::shared_ptr<T> current_;
    bool dropped_ = false;
};

#endif
