#ifndef UNNAMED_PROJECT_SMARTITERATOR_H
#define UNNAMED_PROJECT_SMARTITERATOR_H

/*
 * Wrapper around an iterator that holds a smartpointer, that will give a regular pointer only
 */
template <typename Iterator>
class smart_iterator
{
public:
    typedef smart_iterator<Iterator> this_type;

    typedef typename Iterator::value_type::pointer value_type;

    smart_iterator(Iterator it) : it_{it} {}

    value_type operator *()
    {
        return it_->get();
    }

    this_type &operator++()
    {
        ++it_;
        return *this;
    }

    bool operator ==(const this_type &other)
    {
        return this->it_ == other.it_;
    }

    bool operator !=(const this_type &other)
    {
        return this->it_ != other.it_;
    }

private:
    Iterator it_;
};

template<typename Iterator>
class range
{
public:
    range(Iterator b, Iterator e) : begin_(b), end_(e) {}

    Iterator begin() { return begin_; }
    Iterator end() { return end_; }

private:
    Iterator begin_;
    Iterator end_;
};


#endif // UNNAMED_PROJECT_SMARTITERATOR_H
