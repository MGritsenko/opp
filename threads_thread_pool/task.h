#pragma once

class Task {
public:
	virtual ~Task() {}

	virtual void execute() = 0;
};
