#pragma once

// Common to model{Inps,Outps,Var}

class Iov;
class QHBoxLayout;
class QLineEdit;
class QComboBox;

class RowDesc {
public:
  RowDesc( Iov* io, QHBoxLayout *row_layout, QLineEdit* name_selector, QComboBox* type_selector, QComboBox* stim_selector = nullptr) :
    io(io), row_layout(row_layout), name_selector(name_selector), type_selector(type_selector), stim_selector(stim_selector)
    {}
  ~RowDesc() {}
    
  Iov* io;
  QHBoxLayout *row_layout;
  QLineEdit* name_selector;
  QComboBox* type_selector;
  QComboBox* stim_selector;
};
