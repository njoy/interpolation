template< typename Table, typename Algorithm >
auto do_integrate( const Xdata& xLow, const Xdata& xHi, Algorithm&& searchAlgorithm ) const {
  auto x_low = xLow;
  auto x_hi = xHi;

  bool inverted = x_low > x_hi;
  if(inverted) {
    const auto x_low_tmp = x_low;
    x_low = x_hi;
    x_hi = x_low_tmp;
  }

  // Initialize to zero by doing integral over range of 0 width
  auto integral = this->interpolant().integrate(x_low, x_low,
                                        this->x().front(), this->x().back(),
                                        this->y().front(), this->y().back());

  // Integration may only be carried out over the function's valid domain
  if (x_low <= this->tableMin())
    x_low = this->tableMin();
  else if (x_low >= this->tableMin())
    x_low = this->tableMax();

  if (x_hi >= this->tableMax())
    x_hi = this->tableMax();
  else if (x_hi <= this->tableMin())
    x_hi = this->tableMin();

  // Get iterator for lower bound of first interval
  auto low_it = searchAlgorithm.apply(this->x(), x_low);
  if (*low_it > x_low) low_it = ranges::prev(low_it);

  
  auto x_low_lim = x_low;
  auto x_upp_lim = x_hi;
  bool integrating = true;
  while (integrating) {
    auto hi_it = low_it;
    hi_it = ranges::next(hi_it);

    auto i = ranges::distance(this->x().begin(), low_it);

    auto x1 = *low_it;
    auto x2 = *hi_it;
    auto y1 = *ranges::next(this->y().begin(), i);
    auto y2 = *ranges::next(this->y().begin(), i+1);

    if (x_low_lim < x1) x_low_lim = x1;
    if (x_upp_lim > x2) x_upp_lim = x2;

    integral += this->interpolant().integrate(x_low_lim, x_upp_lim, x1, x2, y1, y2);

    if (x_upp_lim == x_hi)
      integrating = false;
    else {
      x_low_lim = x_upp_lim;
      x_upp_lim = x_hi;
      ranges::next(low_it);
    }
  }

  if(inverted) integral *= -1.;

  return integral;

}