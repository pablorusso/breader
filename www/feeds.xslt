<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

	<xsl:output method="html"/>

	<xsl:template match="feeds">
		<table cellspacing="0" cellpadding="0" border="0" id="selectors-box3" class="round-box">
			<tbody>
				<tr>
					<td class="s tl"/>
					<td class="s"/>
					<td class="s tr"/>
				</tr>
				<tr>
					<td class="s"/>
					<td class="c">
						<ul>
							<div class="sub-tree-header">Suscripciones</div>
							<table>
								<xsl:apply-templates select="feed"/>
							</table>
						</ul>
					</td>
					<td class="s"/>
				</tr>
				<tr>
					<td class="s bl"/>
					<td class="s"/>
					<td class="s br"/>
				</tr>
			</tbody>
		</table>
	</xsl:template>

	<xsl:template match="feed">
		<tr>
			<td class="menuName">
				<a class="link">
					<xsl:attribute name="onclick">
						doAction('?actionCode=F2&amp;params=feedId%23<xsl:value-of select="@id"/>', delFeedHandler, '' )
					</xsl:attribute>
					<img style="border:0" src="images/action_feed_del.png" title="Borrar feed" alt="Borrar feed"/>
				</a>
			</td>

			<td class="menuName">
				<a class="link">
					<xsl:attribute name="onclick">
						doAction('?actionCode=F3&amp;params=feedId%23<xsl:value-of select="@id"/>', feedsHandler, '' )
					</xsl:attribute>
					<img style="border:0" src="images/action_feed_refresh.png" title="Actualizar feed" alt="Actualizar feed"/>
				</a>
			</td>

			<td class="menuName">
				<a class="link">
					<xsl:attribute name="onclick">
						doAction('?actionCode=A5&amp;params=feedId%23<xsl:value-of select="@id"/>', bodyHandler, 'A' )
					</xsl:attribute>
					<xsl:value-of select="@name"/>
				</a>
			</td>
		</tr>
	</xsl:template>
</xsl:transform>