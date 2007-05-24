<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

	<xsl:output method="html"/>

	<xsl:template match="articles">
		<!--html>
			<head>
				<link href="styles/style_common.css" rel="stylesheet" type="text/css"/>
				<link href="styles/style_body.css" rel="stylesheet" type="text/css"/>
			</head>

			<body-->
				<table>
					<tr>
						<td>
							<div id="entries" >
								<xsl:apply-templates select="article"/>
							</div>
						</td>
					</tr>
				</table>
			<!--/body>
		</html-->
	</xsl:template>

	<xsl:template match="article">
		<div class="entry">
			<table class="card">
				<tr>
					<td class="ctl"/>
					<td class="ct"/>
					<td class="ctr"/>
				</tr>
				<tr>
					<td class="cl"/>

					<td class="cc">
						<div class="entry-container entry-main">
							<!--fecha-->
							<div class="entry-date">
								<xsl:value-of select="@date"/>
							</div>

							<!--titulo y link-->
							<h2 class="entry-title">

								<a target="_blank" class="entry-title-link">
								<xsl:attribute name="href">
									<xsl:value-of select="@link"/>
								</xsl:attribute>

								<xsl:value-of select="@title"/>
								<img width="18" height="18" class="entry-title-go-to" src="images/action_go_to.gif"/>
								</a>
							</h2>

							<!--autor-->
							<div class="entry-author">
								from <xsl:value-of select="@author"/>
							</div>

							<!--resumen-->
							<div class="entry-body">
								<xsl:value-of select="summary"/>
							</div>
						</div>
					</td>

					<td class="cr"/>
				</tr>

				<tr class="card-actionrow">
					<td class="cal"/>

					<td class="ca">
						<div class="entry-actions">
							<span class="star link">Agregar a favoritos</span>
							<span class="star link">Marcar como leido</span>
							<span class="star">
								<a href=""><img border="0" src="images/action_add.png"/></a> Categorias:
								<xsl:apply-templates select="tags"/>
							</span>
						</div>
					</td>

					<td class="car"/>
				</tr>

				<tr class="card-bottomrow">
					<td class="cbl"> </td>
					<td class="cb"> </td>
					<td class="cbr"> </td>
				</tr>

			</table>
		</div>
	</xsl:template>

	<xsl:template match="tags">
		<xsl:for-each select="tag">
			<xsl:if test="position() &gt; 1">
				<xsl:text>,   </xsl:text>
			</xsl:if>
			<xsl:value-of select="@name"/><xsl:text> </xsl:text><a href=""><img border="0" src="images/action_check.png"/></a><xsl:text> </xsl:text><a href=""><img border="0" src="images/action_del.png"/></a>
		</xsl:for-each>
	</xsl:template>
</xsl:transform>