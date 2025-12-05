#include <iostream>

namespace topit {
  struct p_t {
    int x, y;
  };
  struct f_t {
    p_t aa, bb;
  }; // pod

  struct IDraw {
    virtual ~IDraw() = default;
    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
  };

  struct Dot : IDraw {
    Dot(int x, int y);
    explicit Dot(p_t dd);
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t d;
  };

  struct Line : IDraw
  {
    Line(p_t s, p_t e);
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t start, end;
  };

  struct Rectangle : IDraw
  {
    Rectangle(p_t upl, p_t botr);
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t upperLeft, bottomRight;
  };

  size_t points(const IDraw &d, p_t **pts, size_t &s);
  f_t frame(const p_t *pts, size_t s);
  char *canvas(f_t fr, char fill);
  void paint(char *cnv, f_t fr, p_t p, char fill);

  bool operator==(p_t, p_t);
  bool operator!=(p_t, p_t);
} // namespace topit

int main() {
  using topit::Dot;
  using topit::f_t;
  using topit::IDraw;
  using topit::p_t;
  IDraw *shps[3] = {};
  p_t *pts = nullptr;
  int err = 0;
  size_t s = 0;
  try {
    shps[0] = new Dot(0, 0);
    shps[1] = new Dot(5, 7);
    shps[2] = new Dot(-5, -2);
    // TODO:
    for (size_t i = 0; i < 3; ++i) {
      s += points(*(shps[i]), &pts, s);
    }
    f_t fr = frame(pts, s); // огран прямоугольник
    char *cnv = canvas(fr, '.');
    for (size_t i; i < s; ++i) {
      paint(cnv, fr, pts[i], '#');
    }
    // [4] нарисовать на полотне все точки(которые достали из фигур)
    // - будем рисовать '#'
    // [5] вывести полотно на экран
    delete[] cnv;
  } catch (...) {
    err = 2;
    std::cerr << "Bad impl";
  }
  delete[] pts;
  delete shps[0];
  delete shps[1];
  delete shps[2];
  return err;
}

topit::Dot::Dot(p_t dd) : IDraw(), d{dd} {}
topit::Dot::Dot(int x, int y) : IDraw(), d{x, y} {}
topit::p_t topit::Dot::begin() const { return d; }
topit::p_t topit::Dot::next(p_t prev) const {
  if (prev != begin()) {
    throw std::logic_error("bad impl");
  }
  return d;
}
bool topit::operator==(p_t a, p_t b) { return a.x == b.x && a.y == b.y; }
bool topit::operator!=(p_t a, p_t b) { return !(a == b); }
topit::Rectangle::Rectangle(p_t upl, p_t botr) : upperLeft(upl), bottomRight(botr){}

topit::p_t topit::Rectangle::begin() const
{
  return upperLeft;
}

topit::p_t topit::Rectangle::next(p_t prev) const
{
  if (prev.y == upperLeft.y && upperLeft.x <= prev.x && prev.x < bottomRight.x)
  {
    return {prev.x + 1, prev.y};
  }
  if (prev.x == bottomRight.x && upperLeft.y >= prev.y && prev.y > bottomRight.y)
  {
    return {prev.x, prev.y - 1};
  }
  if (prev.y == bottomRight.y && upperLeft.x < prev.x && prev.x <= bottomRight.x)
  {
    return {prev.x - 1, prev.y};
  }
  if (prev.x == upperLeft.x && upperLeft.y > prev.y && prev.y >= bottomRight.y)
  {
    return {prev.x, prev.y + 1};
  }
  throw std::logic_error("bad impl");
}

topit::Line::Line(p_t s, p_t e) : IDraw(), start(s), end(e){}

topit::p_t topit::Line::begin() const
{
  return start;
}

topit::p_t topit::Line::next(p_t prev) const
{
  if (prev == end)
  {
    return start;
  }
  if (prev.y == start.y && start.x <= prev.x && prev.x < end.x)
  {
    return {prev.x + 1, prev.y};
  }
  throw std::logic_error("bad impl");
}
