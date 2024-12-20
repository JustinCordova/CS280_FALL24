#include "val.h"

Value Value::operator+(const Value& op) const {
    if (IsInt() && op.IsInt()) {
        return Value(this->Itemp + op.Itemp);
    }
    if (this->IsInt() && op.IsReal()) {
        return Value(this->Itemp + op.Rtemp);
    }
    if (this->IsInt() && op.IsChar()) {
        return Value(this->Itemp + op.Ctemp);
    }
    if (this->IsReal() && op.IsReal()) {
        return Value(this->Rtemp + op.Rtemp);
    }
    if (this->IsReal() && op.IsInt()) {
        return Value(this->Rtemp + op.Itemp);
    }
    if (this->IsReal() && op.IsChar()) {
        return Value(this->Rtemp + op.Ctemp);
    }
    if (this->IsString() && op.IsString()) {
        return Value(this->Stemp + op.Stemp);
    }
    if (this->IsString() && op.IsChar()) {
        return Value(this->Stemp + op.Ctemp);
    }
    if (this->IsChar() && op.IsChar()) {
        return Value(this->Ctemp + op.Ctemp);
    }
    if (this->IsChar() && op.IsInt()) {
        return Value(this->Ctemp + op.Itemp);
    }
    if (this->IsChar() && op.IsReal()) {
        return Value(this->Ctemp + op.Rtemp);
    }
    if (this->IsChar() && op.IsString()) {
        return Value(this->Ctemp + op.Stemp);
    }
    return Value();
}

Value Value::operator-(const Value& op) const {
    if (IsInt() && op.IsInt()) {
        return Value(this->Itemp - op.Itemp);
    }
    if (this->IsInt() && op.IsReal()) {
        return Value(this->Itemp - op.Rtemp);
    }
    if (this->IsInt() && op.IsChar()) {
        return Value(this->Itemp - op.Ctemp);
    }
    if (this->IsReal() && op.IsReal()) {
        return Value(this->Rtemp - op.Rtemp);
    }
    if (this->IsReal() && op.IsInt()) {
        return Value(this->Rtemp - op.Itemp);
    }
    if (this->IsReal() && op.IsChar()) {
        return Value(this->Rtemp - op.Ctemp);
    }
    if (this->IsChar() && op.IsChar()) {
        return Value(this->Ctemp - op.Ctemp);
    }
    if (this->IsChar() && op.IsInt()) {
        return Value(this->Ctemp - op.Itemp);
    }
    if (this->IsChar() && op.IsReal()) {
        return Value(this->Ctemp - op.Rtemp);
    }
    return Value();
}

Value Value::operator*(const Value& op) const {
    if (IsInt() && op.IsInt()) {
        return Value(this->Itemp * op.Itemp);
    }
    if (this->IsInt() && op.IsReal()) {
        return Value(this->Itemp * op.Rtemp);
    }
    if (this->IsInt() && op.IsChar()) {
        return Value(this->Itemp * op.Ctemp);
    }
    if (this->IsReal() && op.IsReal()) {
        return Value(this->Rtemp * op.Rtemp);
    }
    if (this->IsReal() && op.IsInt()) {
        return Value(this->Rtemp * op.Itemp);
    }
    if (this->IsReal() && op.IsChar()) {
        return Value(this->Rtemp * op.Ctemp);
    }
    if (this->IsChar() && op.IsChar()) {
        return Value(this->Ctemp * op.Ctemp);
    }
    if (this->IsChar() && op.IsInt()) {
        return Value(this->Ctemp * op.Itemp);
    }
    if (this->IsChar() && op.IsReal()) {
        return Value(this->Ctemp * op.Rtemp);
    }
    return Value();
}

Value Value::operator/(const Value& op) const {
    if (IsInt() && op.IsInt()) {
        return Value(this->Itemp / op.Itemp);
    }
    if (this->IsInt() && op.IsReal()) {
        return Value(this->Itemp / op.Rtemp);
    }
    if (this->IsInt() && op.IsChar()) {
        return Value(this->Itemp / op.Ctemp);
    }
    if (this->IsReal() && op.IsReal()) {
        return Value(this->Rtemp / op.Rtemp);
    }
    if (this->IsReal() && op.IsInt()) {
        return Value(this->Rtemp / op.Itemp);
    }
    if (this->IsReal() && op.IsChar()) {
        return Value(this->Rtemp / op.Ctemp);
    }
    if (this->IsChar() && op.IsChar()) {
        return Value(this->Ctemp / op.Ctemp);
    }
    if (this->IsChar() && op.IsInt()) {
        return Value(this->Ctemp / op.Itemp);
    }
    if (this->IsChar() && op.IsReal()) {
        return Value(this->Ctemp / op.Rtemp);
    }
    return Value();
}

Value Value::operator%(const Value& op) const {
    if (IsInt() && op.IsInt()) {
        return Value(this->Itemp % op.Itemp);
    }
    if (this->IsInt() && op.IsChar()) {
        return Value(this->Itemp % op.Ctemp);
    }
    if (this->IsChar() && op.IsChar()) {
        return Value(this->Ctemp % op.Ctemp);
    }
    if (this->IsChar() && op.IsInt()) {
        return Value(this->Ctemp % op.Itemp);
    }
    return Value();
}

Value Value::operator==(const Value& op) const {
    if (this->IsInt() && op.IsInt()) {
        return Value(this->Itemp == op.Itemp);
    }
    if (this->IsInt() && op.IsReal()) {
        return Value(this->Itemp == op.Rtemp);
    }
    if (this->IsInt() && op.IsChar()) {
        return Value(this->Itemp == op.Ctemp);
    }
    if (this->IsReal() && op.IsReal()) {
        return Value(this->Rtemp == op.Rtemp);
    }
    if (this->IsReal() && op.IsInt()) {
        return Value(this->Rtemp == op.Itemp);
    }
    if (this->IsString() && op.IsString()) {
        return Value(this->Stemp == op.Stemp);
    }
    if (this->IsBool() && op.IsBool()) {
        return Value(this->Btemp == op.Btemp);
    }
    if (this->IsBool() && op.IsInt()) {
        return Value(this->Btemp == op.Itemp);
    }
    if (this->IsBool() && op.IsReal()) {
        return Value(this->Btemp == op.Rtemp);
    }
    if (this->IsChar() && op.IsChar()) {
        return Value(this->Ctemp == op.Ctemp);
    }
    if (this->IsChar() && op.IsInt()) {
        return Value(this->Ctemp == op.Itemp);
    }
    return Value();
}

Value Value::operator!=(const Value& op) const {
    if (this->IsInt() && op.IsInt()) {
        return Value(this->Itemp != op.Itemp);
    }
    if (this->IsInt() && op.IsReal()) {
        return Value(this->Itemp != op.Rtemp);
    }
    if (this->IsInt() && op.IsChar()) {
        return Value(this->Itemp != op.Ctemp);
    }
    if (this->IsReal() && op.IsReal()) {
        return Value(this->Rtemp != op.Rtemp);
    }
    if (this->IsReal() && op.IsInt()) {
        return Value(this->Rtemp != op.Itemp);
    }
    if (this->IsString() && op.IsString()) {
        return Value(this->Stemp != op.Stemp);
    }
    if (this->IsBool() && op.IsBool()) {
        return Value(this->Btemp != op.Btemp);
    }
    if (this->IsChar() && op.IsChar()) {
        return Value(this->Ctemp != op.Ctemp);
    }
    if (this->IsChar() && op.IsInt()) {
        return Value(this->Ctemp != op.Itemp);
    }
    return Value();
}

Value Value::operator>(const Value& op) const {
    if (this->IsInt() && op.IsInt()) {
        return Value(this->Itemp > op.Itemp);
    }
    if (this->IsInt() && op.IsReal()) {
        return Value(this->Itemp > op.Rtemp);
    }
    if (this->IsInt() && op.IsChar()) {
        return Value(this->Itemp > op.Ctemp);
    }
    if (this->IsReal() && op.IsReal()) {
        return Value(this->Rtemp > op.Rtemp);
    }
    if (this->IsReal() && op.IsInt()) {
        return Value(this->Rtemp > op.Itemp);
    }
    if (this->IsChar() && op.IsInt()) {
        return Value(this->Ctemp > op.Itemp);
    }
    if (this->IsChar() && op.IsChar()) {
        return Value(this->Ctemp > op.Ctemp);
    }
    if (this->IsString() && op.IsString()) {
        return Value(this->Stemp > op.Stemp);
    }
    if (this->IsBool() && op.IsBool()) {
        return Value(this->Btemp > op.Btemp);
    }
    return Value();
}

Value Value::operator<(const Value& op) const {
    if (this->IsInt() && op.IsInt()) {
        return Value(this->Itemp < op.Itemp);
    }
    if (this->IsInt() && op.IsReal()) {
        return Value(this->Itemp < op.Rtemp);
    }
    if (this->IsInt() && op.IsChar()) {
        return Value(this->Itemp < op.Ctemp);
    }
    if (this->IsReal() && op.IsReal()) {
        return Value(this->Rtemp < op.Rtemp);
    }
    if (this->IsReal() && op.IsInt()) {
        return Value(this->Rtemp < op.Itemp);
    }
    if (this->IsChar() && op.IsInt()) {
        return Value(this->Ctemp < op.Itemp);
    }
    if (this->IsChar() && op.IsChar()) {
        return Value(this->Ctemp < op.Ctemp);
    }
    if (this->IsString() && op.IsString()) {
        return Value(this->Stemp < op.Stemp);
    }
    if (this->IsBool() && op.IsBool()) {
        return Value(this->Btemp < op.Btemp);
    }
    return Value();
}

Value Value::operator&&(const Value& op) const {
    if (this->IsBool() && op.IsBool()) {
        return Value(this->Btemp && op.Btemp);
    }
    return Value();
}

Value Value::operator||(const Value& op) const {
    if (this->IsBool() && op.IsBool()) {
        return Value(this->Btemp || op.Btemp);
    }
    return Value();
}

Value Value::operator!() const {
    if (this->IsBool()) {
        return Value(!this->Btemp);
    }
    return Value();
}