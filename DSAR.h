#include <iostream>
#include <type_traits>

template<bool T, typename left, typename right>
struct conditional : left
{
    constexpr conditional() = default;

    template<typename Q>
    constexpr conditional(Q q) : left(std::forward<Q>(q))
    {
    }
}; 

template<typename left, typename right>
struct conditional<false, left, right> : right
{
    constexpr conditional() = default;

    template<typename Q>
    constexpr conditional(Q q) : right(std::forward<Q>(q))
    {
    }
};

template<int a>
struct val;

template<int... a>
struct vals;

template<int lower, int upper>
struct ranged_val;

template<typename... items>
struct minimum
{};

template<int left, int right>
struct minimum<val<left>, val<right>> : public 
    val<conditional<
        val<left>::value <= val<right>::value,
        val<left>,
        val<right
        >
    >::value>
{
    template<typename T>
    constexpr minimum(T&& t) : 
    val<conditional<val<left>::value <= val<right>::value, val<left>, val<right>>::value>(std::forward<T>(t))
    {
    }
};

template<int left, int right, int next, typename... items>
struct minimum<val<left>, val<right>, val<next>, items...> : public 
    minimum<
        val<conditional<
            val<left>::value <= val<right>::value,
            val<left>,
            val<right
            >
        >::value>,
        val<next>,
        items...
    >
{
    template<typename T>
    constexpr minimum(T&& t) : 
    minimum<
        val<conditional<
            val<left>::value <= val<right>::value,
            val<left>,
            val<right
            >
        >::value>,
        val<next>,
        items...
    >(std::forward<T>(t))
    {
    }
};

template<int left_lower, int left_upper, int right_lower, int right_upper>
struct minimum<ranged_val<left_lower, left_upper>, ranged_val<right_lower, right_upper>>
    : public conditional<ranged_val<left_lower, left_upper>::upperbound <= ranged_val<right_lower, right_upper>::upperbound, ranged_val<left_lower, left_upper>, ranged_val<right_lower, right_upper>>
{
    template<typename T>
    constexpr minimum(T&& t) : 
    conditional<ranged_val<left_lower, left_upper>::upperbound <= ranged_val<right_lower, right_upper>::upperbound, ranged_val<left_lower, left_upper>, ranged_val<right_lower, right_upper>>(std::forward<T>(t))
    {
    }
};

template<typename... items>
struct maximum
{};

template<int left, int right>
struct maximum<val<left>, val<right>> : public 
    val<conditional<
        val<left>::value >= val<right>::value,
        val<left>,
        val<right
        >
    >::value>
{
    template<typename T>
    constexpr maximum(T&& t) : 
    val<conditional<val<left>::value >= val<right>::value, val<left>, val<right>>::value>(std::forward<T>(t))
    {
    }
};

template<int left, int right, int next, typename... items>
struct maximum<val<left>, val<right>, val<next>, items...> : public 
    maximum<
        val<conditional<
            val<left>::value >= val<right>::value,
            val<left>,
            val<right
            >
        >::value>,
        val<next>,
        items...
    >
{
    template<typename T>
    constexpr maximum(T&& t) : 
    maximum<
        val<conditional<
            val<left>::value >= val<right>::value,
            val<left>,
            val<right
            >
        >::value>,
        val<next>,
        items...
    >(std::forward<T>(t))
    {
    }
};

template<int left_lower, int left_upper, int right_lower, int right_upper>
struct maximum<ranged_val<left_lower, left_upper>, ranged_val<right_lower, right_upper>>
    : public conditional<ranged_val<left_lower, left_upper>::upperbound >= ranged_val<right_lower, right_upper>::upperbound, ranged_val<left_lower, left_upper>, ranged_val<right_lower, right_upper>>
{
    template<typename T>
    constexpr maximum(T&& t) : 
    conditional<ranged_val<left_lower, left_upper>::upperbound >= ranged_val<right_lower, right_upper>::upperbound, ranged_val<left_lower, left_upper>, ranged_val<right_lower, right_upper>>(std::forward<T>(t))
    {
    }
};

struct unsafe_val
{
    int value = 0;

    explicit constexpr unsafe_val(int _) noexcept : value(_)
    {
    }
};

template<typename T>
static constexpr T unsafe_cast(int value) noexcept
{
    return T{value};
}

template<typename... items>
struct add
{
    constexpr add() = default;
};

template<int left, int right>
struct add<val<left>, val<right>> : public val<val<left>::value + val<right>::value>
{
    constexpr add() = default;

    template<typename Q>
    constexpr add(Q q) : val<val<left>::value + val<right>::value>(std::forward<Q>(q))
    {
    }
};

template<int left, int right_lower, int right_upper>
struct add<val<left>, ranged_val<right_lower, right_upper>> : public
	ranged_val<
		ranged_val<right_lower, right_upper>::lowerbound + left,
		ranged_val<right_lower, right_upper>::upperbound + left
	>
{
    constexpr add() = default;

    template<typename Q>
    constexpr add(Q q) : 
	ranged_val<
		ranged_val<right_lower, right_upper>::lowerbound + left,
		ranged_val<right_lower, right_upper>::upperbound + left
	>(std::forward<Q>(q))
    {
    }
};

template<int left_lower, int left_upper, int right>
struct add<ranged_val<left_lower, left_upper>, val<right>> : public
	ranged_val<
		ranged_val<left_lower, left_upper>::lowerbound + right,
		ranged_val<left_lower, left_upper>::upperbound + right
	>
{
    constexpr add() = default;

    template<typename Q>
    constexpr add(Q q) : 
	ranged_val<
		ranged_val<left_lower, left_upper>::lowerbound + right,
		ranged_val<left_lower, left_upper>::upperbound + right
	>(std::forward<Q>(q))
    {
    }
};

template<int left_lower, int left_upper, int right_lower, int right_upper>
struct add<ranged_val<left_lower, left_upper>, ranged_val<right_lower, right_upper>>
    : public ranged_val<
        ranged_val<left_lower, left_upper>::lowerbound
        +
        ranged_val<right_lower, right_upper>::lowerbound,
        ranged_val<left_lower, left_upper>::upperbound
        +
        ranged_val<right_lower, right_upper>::upperbound>
{
    constexpr add() = default;

    template<typename Q>
    constexpr add(Q q) : ranged_val<
        ranged_val<left_lower, left_upper>::lowerbound
        +
        ranged_val<right_lower, right_upper>::lowerbound,
        ranged_val<left_lower, left_upper>::upperbound
        +
        ranged_val<right_lower, right_upper>::upperbound>(std::forward<Q>(q))
    {
    }
};

template<int left, typename right, typename next, typename... items>
struct add<val<left>, right, next, items...> : public 
    add<
		add<
			val<left>,
			right
		>,
		next,
		items...
	>
{
    constexpr add() = default;

    template<typename Q>
    constexpr add(Q q) : add<
		add<
			val<left>,
			right
		>,
		next,
		items...
	>(std::forward<Q>(q))
    {
    }
};

template<int left_lower, int left_upper, typename right, typename next, typename... items>
struct add<ranged_val<left_lower, left_upper>, right, next, items...>
    : public add<
        add<
			ranged_val<left_lower, left_upper>,
			right
		>,
        next,
        items...
    >
{
    constexpr add() = default;

    template<typename Q>
    constexpr add(Q q) : add<
        add<
			ranged_val<left_lower, left_upper>,
			right
		>,
        next,
        items...
    >(std::forward<Q>(q))
    {
    }
};

template<typename... items>
struct minus
{
    constexpr minus() = default;
};

template<int left, int right>
struct minus<val<left>, val<right>> : public val<val<left>::value - val<right>::value>
{
    constexpr minus() = default;

    template<typename Q>
    constexpr minus(Q q) : val<val<left>::value - val<right>::value>(std::forward<Q>(q))
    {
    }
};

template<int left, int right_lower, int right_upper>
struct minus<val<left>, ranged_val<right_lower, right_upper>> : public
	ranged_val<
		ranged_val<right_lower, right_upper>::lowerbound - left,
		ranged_val<right_lower, right_upper>::upperbound - left
	>
{
    constexpr minus() = default;

    template<typename Q>
    constexpr minus(Q q) : 
	ranged_val<
		ranged_val<right_lower, right_upper>::lowerbound - left,
		ranged_val<right_lower, right_upper>::upperbound - left
	>(std::forward<Q>(q))
    {
    }
};

template<int left_lower, int left_upper, int right>
struct minus<ranged_val<left_lower, left_upper>, val<right>> : public
	ranged_val<
		ranged_val<left_lower, left_upper>::lowerbound - right,
		ranged_val<left_lower, left_upper>::upperbound - right
	>
{
    constexpr minus() = default;

    template<typename Q>
    constexpr minus(Q q) : 
	ranged_val<
		ranged_val<left_lower, left_upper>::lowerbound - right,
		ranged_val<left_lower, left_upper>::upperbound - right
	>(std::forward<Q>(q))
    {
    }
};

template<int left_lower, int left_upper, int right_lower, int right_upper>
struct minus<ranged_val<left_lower, left_upper>, ranged_val<right_lower, right_upper>>
    : public ranged_val<
        ranged_val<left_lower, left_upper>::lowerbound
        -
        ranged_val<right_lower, right_upper>::lowerbound,
        ranged_val<left_lower, left_upper>::upperbound
        -
        ranged_val<right_lower, right_upper>::upperbound>
{
    constexpr minus() = default;

    template<typename Q>
    constexpr minus(Q q) : ranged_val<
        ranged_val<left_lower, left_upper>::lowerbound
        -
        ranged_val<right_lower, right_upper>::lowerbound,
        ranged_val<left_lower, left_upper>::upperbound
        -
        ranged_val<right_lower, right_upper>::upperbound>(std::forward<Q>(q))
    {
    }
};

template<int left, typename right, typename next, typename... items>
struct minus<val<left>, right, next, items...> : public 
    minus<
		minus<
			val<left>,
			right
		>,
		next,
		items...
	>
{
    constexpr minus() = default;

    template<typename Q>
    constexpr minus(Q q) : minus<
		minus<
			val<left>,
			right
		>,
		next,
		items...
	>(std::forward<Q>(q))
    {
    }
};

template<int left_lower, int left_upper, typename right, typename next, typename... items>
struct minus<ranged_val<left_lower, left_upper>, right, next, items...>
    : public minus<
        minus<
			ranged_val<left_lower, left_upper>,
			right
		>,
        next,
        items...
    >
{
    constexpr minus() = default;

    template<typename Q>
    constexpr minus(Q q) : minus<
        minus<
			ranged_val<left_lower, left_upper>,
			right
		>,
        next,
        items...
    >(std::forward<Q>(q))
    {
    }
};

template<typename... items>
struct multiply
{
    constexpr multiply() = default;
};

template<int left, int right>
struct multiply<val<left>, val<right>> : public val<val<left>::value * val<right>::value>
{
    constexpr multiply() = default;

    template<typename Q>
    constexpr multiply(Q q) : val<val<left>::value * val<right>::value>(std::forward<Q>(q))
    {
    }
};

template<int left, int right_lower, int right_upper>
struct multiply<val<left>, ranged_val<right_lower, right_upper>> : public 
	ranged_val<
		ranged_val<right_lower, right_upper>::lowerbound * left,
		ranged_val<right_lower, right_upper>::upperbound * left
	>
{
    constexpr multiply() = default;

    template<typename Q>
    constexpr multiply(Q q) : 
	ranged_val<
		ranged_val<right_lower, right_upper>::lowerbound * left,
		ranged_val<right_lower, right_upper>::upperbound * left
	>(std::forward<Q>(q))
    {
    }
};

template<int left_lower, int left_upper, int right>
struct multiply<ranged_val<left_lower, left_upper>, val<right>> : public 
	ranged_val<
		ranged_val<left_lower, left_upper>::lowerbound * right,
		ranged_val<left_lower, left_upper>::upperbound * right
	>
{
    constexpr multiply() = default;

    template<typename Q>
    constexpr multiply(Q q) : 
	ranged_val<
		ranged_val<left_lower, left_upper>::lowerbound * right,
		ranged_val<left_lower, left_upper>::upperbound * right
	>(std::forward<Q>(q))
    {
    }
};

template<int left_lower, int left_upper, int right_lower, int right_upper>
struct multiply<ranged_val<left_lower, left_upper>, ranged_val<right_lower, right_upper>>
    : public ranged_val<
        minimum<
            val<ranged_val<left_lower, left_upper>::lowerbound
            *
            ranged_val<right_lower, right_upper>::lowerbound>,
            val<ranged_val<left_lower, left_upper>::lowerbound
            *
            ranged_val<right_lower, right_upper>::upperbound>,
            val<ranged_val<left_lower, left_upper>::upperbound
            *
            ranged_val<right_lower, right_upper>::upperbound>,
            val<ranged_val<left_lower, left_upper>::upperbound
            *
            ranged_val<right_lower, right_upper>::lowerbound>
        >::value,
        maximum<
            val<ranged_val<left_lower, left_upper>::lowerbound
            *
            ranged_val<right_lower, right_upper>::lowerbound>,
            val<ranged_val<left_lower, left_upper>::lowerbound
            *
            ranged_val<right_lower, right_upper>::upperbound>,
            val<ranged_val<left_lower, left_upper>::upperbound
            *
            ranged_val<right_lower, right_upper>::upperbound>,
            val<ranged_val<left_lower, left_upper>::upperbound
            *
            ranged_val<right_lower, right_upper>::lowerbound>
        >::value>
{
    constexpr multiply() = default;

    template<typename Q>
    constexpr multiply(Q q) : ranged_val<
        minimum<
            val<ranged_val<left_lower, left_upper>::lowerbound
            *
            ranged_val<right_lower, right_upper>::lowerbound>,
            val<ranged_val<left_lower, left_upper>::lowerbound
            *
            ranged_val<right_lower, right_upper>::upperbound>,
            val<ranged_val<left_lower, left_upper>::upperbound
            *
            ranged_val<right_lower, right_upper>::upperbound>,
            val<ranged_val<left_lower, left_upper>::upperbound
            *
            ranged_val<right_lower, right_upper>::lowerbound>
        >::value,
        maximum<
            val<ranged_val<left_lower, left_upper>::lowerbound
            *
            ranged_val<right_lower, right_upper>::lowerbound>,
            val<ranged_val<left_lower, left_upper>::lowerbound
            *
            ranged_val<right_lower, right_upper>::upperbound>,
            val<ranged_val<left_lower, left_upper>::upperbound
            *
            ranged_val<right_lower, right_upper>::upperbound>,
            val<ranged_val<left_lower, left_upper>::upperbound
            *
            ranged_val<right_lower, right_upper>::lowerbound>
        >::value>(std::forward<Q>(q))
    {
    }
};

template<int left, typename right, typename next, typename... items>
struct multiply<val<left>, right, next, items...> : public
	multiply<
		multiply<
			val<left>,
			right
		>,
		next,
		items...
	>
{
    constexpr multiply() = default;

    template<typename Q>
    constexpr multiply(Q q) : 
	multiply<
		multiply<
			val<left>,
			right
		>,
		next,
		items...
	>(std::forward<Q>(q))
    {
    }
};

template<int left_lower, int left_upper, typename right, typename next, typename... items>
struct multiply<ranged_val<left_lower, left_upper>, right, next, items...> : public
	multiply<
		multiply<
			ranged_val<left_lower, left_upper>,
			right
		>,
		next,
		items...
	>
{
    constexpr multiply() = default;

    template<typename Q>
    constexpr multiply(Q q) :
	multiply<
		multiply<
			ranged_val<left_lower, left_upper>,
			right
		>,
		next,
		items...
	>(std::forward<Q>(q))
    {
    }
};

template<typename... items>
struct modulo
{};

template<int left, int right>
struct modulo<val<left>, val<right>> : public val<left % right>
{
	constexpr modulo() = default;
	
	template<typename Q>
	constexpr modulo(Q q) : val<left % right>(std::forward<Q>(q))
	{
	}
};

template<int left_lower, int left_upper, int right>
struct modulo<ranged_val<left_lower, left_upper>, val<right>> : public
	ranged_val<
		ranged_val<left_lower, left_upper>::lowerbound % right,
		maximum<
			val<ranged_val<left_lower, left_upper>::upperbound % right>,
			val<right - 1>
		>::value
	>
{
	constexpr modulo() = default;
	
	template<typename Q>
	constexpr modulo(Q q) :
		ranged_val<
			ranged_val<left_lower, left_upper>::lowerbound % right,
			maximum<
				val<ranged_val<left_lower, left_upper>::upperbound % right>,
				val<right - 1>
			>::value
		>(std::forward<Q>(q))
	{
	}
};

template<int a>
struct val {
    static constexpr auto value = a;

    template<int b>
    constexpr auto operator+(val<b>) const noexcept  -> add<val<a>, val<b>>
    {
        return {};
    }

    template<int b>
    constexpr auto operator-(val<b> rhs) const noexcept -> minus<val<a>, val<b>>
    {
        return {};
    }

    template<int b>
    constexpr auto operator*(val<b> rhs) const noexcept -> multiply<val<a>, val<b>>
    {
        return {};
    }
};

template<int lower, int upper>
struct ranged_val
{
    static constexpr auto lowerbound = conditional<(val<lower>::value <= val<upper>::value), val<lower>, val<upper>>::value;
    static constexpr auto upperbound = conditional<(val<lower>::value >= val<upper>::value), val<lower>, val<upper>>::value;
    int value = lower;

    constexpr ranged_val() = default;

    template<int rhs_lower, int rhs_upper>
    constexpr ranged_val(ranged_val<rhs_lower, rhs_upper> _) noexcept  : value(_.value)
    {
        static_assert(lowerbound <= rhs_lower, "New set must have lower bound <= right hand side lower bound");
        static_assert(upperbound >= rhs_upper, "New set must have upper bound >= right hand side upper bound");
    }

    template<int rhs_value>
    constexpr ranged_val(val<rhs_value>) noexcept  : value(rhs_value)
    {
        static_assert(lowerbound <= rhs_value, "New set must have lower bound <= right hand side value");
        static_assert(upperbound >= rhs_value, "New set must have upper bound >= right hand side value");
    }

    constexpr ranged_val(unsafe_val _) noexcept : value(_.value)
    {
    }

    template<int rhs_lower, int rhs_upper>
    constexpr auto operator+(ranged_val<rhs_lower, rhs_upper> _) const noexcept -> add<ranged_val<lower, upper>, ranged_val<rhs_lower, rhs_upper>>
    {
        return {unsafe_cast<unsafe_val>(value + _.value)};
    }

    template<int rhs_lower, int rhs_upper>
    constexpr auto operator-(ranged_val<rhs_lower, rhs_upper> _) const noexcept -> minus<ranged_val<lower, upper>, ranged_val<rhs_lower, rhs_upper>>
    {
        return {unsafe_cast<unsafe_val>(value - _.value)};
    }

    template<int rhs_lower, int rhs_upper>
    constexpr auto operator*(ranged_val<rhs_lower, rhs_upper> _) const noexcept -> multiply<ranged_val<lower, upper>, ranged_val<rhs_lower, rhs_upper>>
    {
        return {unsafe_cast<unsafe_val>(value * _.value)};
    }

    template<int rhs_lower, int rhs_upper>
    constexpr auto operator=(ranged_val<rhs_lower, rhs_upper> _) noexcept  -> ranged_val<lower, upper>*
    {
        static_assert(lowerbound <= rhs_lower, "New set must have lower bound <= right hand side lower bound");
        static_assert(upperbound >= rhs_upper, "New set must have upper bound >= right hand side upper bound");

        value = _.value;
        return *this;
    }

    template<int rhs_value>
    constexpr auto operator=(val<rhs_value>) noexcept  -> ranged_val<upper, lower>*
    {
        static_assert(lowerbound <= rhs_value, "New set must have lower bound <= right hand side value");
        static_assert(upperbound >= rhs_value, "New set must have upper bound >= right hand side value");
        
        value = rhs_value;
        return *this;
    }

private:
    constexpr ranged_val(int _) noexcept : value(_)
    {}
};