[System.Serializable]
public class Fluent
{
	public string entityName;
	public string fluentName;

	public float GetValue()
	{
		if (entityName.Length == 0) {
			return (SandCat.instance.GetFluentValue(fluentName));
		} else {
			return (SandCat.instance.GetEntityFluentValue(entityName, fluentName));
		}
	}
}