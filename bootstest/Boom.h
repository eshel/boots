#ifndef _BOOM_H_
#define _BOOM_H_

class Boom : public Animation {
public:
	Boom(MultiNeoPixel& strip, bool enabled) : Animation(strip, enabled) {
		mCX = mCY = 0;
		mRadius = 0.0f;
		mInProgress = false;
		mChanceOf1000 = 10;
	}

	void setChanceOf1000(uint16_t chance) {
		mChanceOf1000 = chance;
	}

	bool shouldExplode() {
		uint16_t chance = random(0, 1000);
		return (chance < mChanceOf1000);
	}

	void randomCenter() {
		mCX = randomX();
		mCY = randomY();
	}

	void expand() {
		
	}


	void beginExpand() {
		mCX = randomX();
		mCY = randomY();
		mRadius = 0.0f;
		mInProgress = true;
	}

	int8_t randomX() {
		return random(0, mStrip.getSizeX());
	}

	int8_t randomY() {
		return random(0, mStrip.getSizeY());
	}

protected:
	virtual void performDraw() {
		if (mInProgress) {
			expand();
		} else {
			if (beginExpand()) {
				explode();
			}
		}
	}

private:
	int8_t mCX;
	int8_t mCY;
	float mRadius;
	uint16_t mChanceOf1000;
	bool mInProgress;
};

#endif // #ifndef _BOOM_H_
