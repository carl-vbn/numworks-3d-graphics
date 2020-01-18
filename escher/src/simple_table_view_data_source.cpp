#include <escher/simple_table_view_data_source.h>
#include <escher/metric.h>

extern "C" {
#include <assert.h>
}

KDCoordinate SimpleTableViewDataSource::columnWidth(int i) {
  return cellWidth();
}

KDCoordinate SimpleTableViewDataSource::rowHeight(int j) {
  return cellHeight();
}

KDCoordinate SimpleTableViewDataSource::cumulatedWidthFromIndex(int i) {
  return cellWidth() * i;
}

KDCoordinate SimpleTableViewDataSource::cumulatedHeightFromIndex(int j) {
  return cellHeight() * j;
}

int SimpleTableViewDataSource::indexFromCumulatedWidth(KDCoordinate offsetX) {
  KDCoordinate width = cellWidth();
  if (width == 0) {
    return 0;
  }
  return (offsetX - 1) / width;
}

int SimpleTableViewDataSource::indexFromCumulatedHeight(KDCoordinate offsetY) {
  KDCoordinate height = cellHeight();
  if (height == 0) {
    return 0;
  }
  return (offsetY - 1) / height;
}

HighlightCell * SimpleTableViewDataSource::reusableCell(int index, int type) {
  assert(type == 0);
  return reusableCell(index);
}

int SimpleTableViewDataSource::reusableCellCount(int type) {
  assert(type == 0);
  return reusableCellCount();
}

int SimpleTableViewDataSource::typeAtLocation(int i, int j) {
  return 0;
}
