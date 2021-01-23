template< typename Table, typename Algorithm >
auto integrate( const Xdata& xL, const Xdata& xH,
                Algorithm&& searchAlgorithm ) const {
  auto xLow = xL;
  auto xHi = xH;

  const bool inverted = xLow > xHi;
  if(inverted) {
    const auto xLowTmp = xLow;
    xLow = xHi;
    xHi = xLowTmp;
  }

  // Integration may only be carried out over the function's valid domain
  if (xLow < this->tableMin()) {
    xLow = this->tableMin();
  } else if (xLow > this->tableMax()) {
    xLow = this->tableMax();
  }

  if (xHi > this->tableMax()) {
    xHi = this->tableMax();
  } else if (xHi < this->tableMin()) {
    xHi = this->tableMin(); 
  }

  // Initialize to zero by doing integral over range of 0 width
  auto integral = (xLow - xLow) * this->y().front();
  if (xLow == xHi) {
    return integral;
  }

  // Get iterator for lower bound of first interval
  auto lowIt = searchAlgorithm.apply(this->x(), xLow);
  if (*lowIt > xLow) {
    lowIt = ranges::prev(lowIt);
  }

  auto xLowLim = xLow;
  auto xUppLim = xHi;
  bool integrating = true;
  while (integrating) {
    auto hiIt = lowIt;
    hiIt = ranges::next(hiIt);

    auto i = ranges::distance(this->x().begin(), low_it);

    auto x1 = *lowIt;
    auto x2 = *hiIt;
    auto y1 = *ranges::next(this->y().begin(), i);
    auto y2 = *ranges::next(this->y().begin(), i+1);

    if (xLowLim < x1) {
      xLowLim = x1;
    }
    if (xUppLim > x2) {
      xUppLim = x2;
    }

    integral += this->interpolant().integrate(xLowLim, xUppLim, x1, x2, y1, y2);

    if (xUppLim == xHi) {
      integrating = false;
    } else {
      xLowLim = xUppLim;
      xUppLim = xHi;
      lowIt = ranges::next(lowIt);
    }
  }

  if(inverted) {
    integral *= -1.;
  }

  return integral;
}