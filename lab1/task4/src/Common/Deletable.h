#pragma once

class Deletable {
public:
    void Delete();
    void Remedy();
    [[nodiscard]] bool IsDeleted() const;
private:
    bool deleted = false;
};
