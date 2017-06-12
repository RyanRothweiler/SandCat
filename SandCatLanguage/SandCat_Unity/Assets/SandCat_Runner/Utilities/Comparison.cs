public enum CompType {
	LessThan, LessThanOrEqualTo, EqualTo, NotEqualTo, GreaterThan, GreaterThanOrEqualTo,
}

[System.Serializable]
public class Comparison
{
	public CompType type;
	public bool IsTrue(int LeftVal, int RightVal)
	{
		if (type == CompType.LessThan) {
			return (LeftVal < RightVal);
		} else if (type == CompType.LessThanOrEqualTo) {
			return (LeftVal <= RightVal);
		} else if (type == CompType.EqualTo) {
			return (LeftVal == RightVal);
		} else if (type == CompType.NotEqualTo) {
			return (LeftVal != RightVal);
		} else if (type == CompType.GreaterThanOrEqualTo) {
			return (LeftVal >= RightVal);
		} else if (type == CompType.GreaterThan) {
			return (LeftVal < RightVal);
		}

		return (true);
	}
}